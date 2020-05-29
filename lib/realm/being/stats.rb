module Realm
  class Being
    BASE_HEALTH     = 100
    MAX_HEALTH_GAIN = 10_000
    MAX_MANA_GAIN   = 10_000
    BASE_MANA       = 100
    MAX_STAMINA     = 100

    BASE_LEVEL   = 1
    BASE_EXP     = 0
    BASE_TNL     = 100
    TNL_GROWTH   = 1.03
    TNL_FOR_HERO = 1_000_000
    BASE_TRAINS  = 5

    NEWBIE   = 1
    TRAVELER = 10
    HERO     = 100
    DEMIGOD  = HERO + 10
    GOD      = HERO + 15
    CREATOR  = HERO + 20

    LEVELS = %w(NEWBIE TRAVELER HERO DEMIGOD GOD CREATOR)

    # Many immortal commands (such as 'force' and 'summon') require that in
    # order to perform them, the actor be a sufficiently higher level than the
    # victim. This is a sort of "chain of command" security measure and this
    # minimum required level difference is codified as IMMORTAL_SENIORITY.
    IMMORTAL_SENIORITY = 5

    FITNESS_BASE      = 15
    FITNESS_THRESHOLD = 20

    DEFENSE_BASE = 0
    OFFENSE_BASE = 0

    BASE_AGE    = 17
    BASE_HEIGHT = 3
    BASE_WEIGHT = 3

    # Core names can be grouped into five distinct buckets.
    EXPERIENCE = %i(level exp tnl training_points)
    WELLNESS   = %i(health mana stamina)
    FITNESS    = %i(strength dexterity constitution intelligence focus creativity charisma luck)
    DEFENSE    = %i(armor bash slash pierce exotic)
    OFFENSE    = %i(hit_bonus damage_bonus)
    PHYSICAL   = %i(age height weight)

    # Discourage direct manipulation of any core names by only defining readers.
    (EXPERIENCE + WELLNESS + FITNESS + DEFENSE + OFFENSE + PHYSICAL).each do |name|
      attr_reader name
    end

    # Shortcuts!
    FITNESS.each { |name| alias_method :"#{name[0..2]}", :"#{name}" }

    # Random attributes of Being that can include static modifiers.
    MODIFYING_ATTRS = %w(klass anatomy)

    attr_accessor :circumstance

    ###########################################################################
    # Set default values for performance attributes.
    after_initialize -> {
      @level           = BASE_LEVEL
      @exp             = BASE_EXP
      @tnl             = BASE_TNL
      @training_points = BASE_TRAINS

      @health  = Mechanics::MaxStat.new BASE_HEALTH
      @mana    = Mechanics::MaxStat.new BASE_MANA
      @stamina = Mechanics::MaxStat.new MAX_STAMINA

      # rubocop:disable Metrics/LineLength
      FITNESS.each { |name| instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(FITNESS_BASE, FITNESS_THRESHOLD) }
      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(DEFENSE_BASE) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(OFFENSE_BASE) }
      # rubocop:enable Metrics/LineLength

      @age    = BASE_AGE
      @height = BASE_HEIGHT
      @weight = BASE_WEIGHT

      @circumstance = nil
    }

    ###########################################################################
    on_hashify -> {
      hash = Hash.new

      # NOTE: Must remove (and save) equipment before saving core stats. Not
      # only does this make it easier to prevent and detect errors with save
      # and restore logic, but it also makes it possible to edit Avatar files
      # (esp. stats and equipment) by hand without worrying about cascading
      # side effects.
      #
      # Unfortunately, this means we have to unload, save, and reload all
      # equipment during Being::Stats#on_hashify.
      saved_eq = {}
      if @anatomy
        hash[:equipment] = Hash[@anatomy.slots.map { |slot|
          next if slot.empty?

          item = slot.set nil
          saved_eq[slot.name] = item
          [slot.name, item.hashify]
        }.compact]
      end

      # NOTE: Same comments as above for equipment, except for the subobjects
      # defined in MODIFYING_ATTRS.
      MODIFYING_ATTRS.each do |x|
        x = send x.to_sym
        next if x.nil?
        modify x.modifiers, true if x.respond_to? :modifiers
      end

      EXPERIENCE.each { |name| hash[name] = send name }

      WELLNESS.each do |name|
        stat = send name
        hash[name] = "#{stat.current}/#{stat.max_stat.baseline}"
      end

      FITNESS.each do |name|
        stat = send name
        hash[name] = "#{stat.current_stat.baseline}:#{stat.threshold}"
      end

      DEFENSE.each { |name| hash[name] = send(name).baseline }
      OFFENSE.each { |name| hash[name] = send(name).baseline }

      PHYSICAL.each { |name| hash[name] = send name }

      # Restore Being attribute modifications.
      restore_static_modifiers

      # Restore the equipment back into position.
      saved_eq.each do |slot_name, item|
        @anatomy.get_empty_slot(slot_name).set item
      end

      { stats: hash }
    }

    ###########################################################################
    on_dehashify ->hash {
      hash = hash[:stats]

      EXPERIENCE.each { |name| instance_variable_set "@#{name}", hash[name] }

      WELLNESS.each do |name|
        cur, max = hash[name].split '/'
        instance_variable_set "@#{name}", Mechanics::MaxStat.new(max.to_i)
        send(name).current = cur.to_i
      end

      FITNESS.each do |name|
        baseline, threshold = hash[name].split ':'
        instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(baseline.to_i, threshold.to_i)
      end

      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }

      PHYSICAL.each { |name| instance_variable_set "@#{name}", hash[name] if hash.has_key? name }

      # Must restore core stats before restoring equipment
      # See comments in on_hashify.
      # TODO: Deduplicate this logic with the [logically] equivalent loop at the end of on_hashify.
      if @anatomy && hash.has_key?(:equipment)
        hash[:equipment].each do |slot_name, h|
          @anatomy.change_equipment @anatomy.get_empty_slot(slot_name), Realm::Item.dehashify(h), nil
        end
      end

      # This is an ugly situation where the MODIFYING_ATTRS have already been
      # set (in the on_dehashify callback defined in being/being.rb), but THIS
      # on_dehashify overrites the stat values. This means we have to manually
      # re-apply any of those modifiers.
      restore_static_modifiers
    }

    ###########################################################################
    vivify -> {
      $world.schedule Mechanics::RecurringJob.new 0, self, 60, :auto_health
      $world.schedule Mechanics::RecurringJob.new 0, self, 60, :auto_mana
      $world.schedule Mechanics::RecurringJob.new 0, self, 3, :auto_stamina
    }

    ###########################################################################
    def restore_static_modifiers
      MODIFYING_ATTRS.each do |x|
        x = send x.to_sym
        next if x.nil?
        modify x.modifiers if x.respond_to? :modifiers
      end
    end

    ###########################################################################
    def newbie?
      @level < TRAVELER
    end

    (LEVELS - %w[NEWBIE]).each do |lvl|
      define_method :"#{lvl.downcase}?", (Proc.new { @level >= Realm::Being.const_get(lvl) })
    end

    ###########################################################################
    def seniority?(victim)
      victim.level < @level - IMMORTAL_SENIORITY
    end

    ###########################################################################
    def level_label
      LEVELS.reverse.find { |lvl| @level >= Realm::Being.const_get(lvl) }.downcase
    end

    ###########################################################################
    def auto_health
      this_heal = Util::Computation.bound health.max / 10, 1, health.current
      health.increase this_heal
    end

    ###########################################################################
    def auto_mana
      this_heal = Util::Computation.bound mana.max / 10, 1, mana.current
      mana.increase this_heal
    end

    ###########################################################################
    def auto_stamina
      this_heal = Util::Computation.bound level / 5, 1, MAX_STAMINA / 2.5
      stamina.increase this_heal
    end

    ###########################################################################
    # NOTE: Avatars stop gaining experience and show 0 tnl once they reach HERO
    def award_experience(exp_gained)
      return if self.is_a?(Avatar) && level >= HERO

      puts 'You gain {Y%d{x experience points!' % exp_gained
      @exp = exp + exp_gained

      while exp_gained > 0
        if exp_gained < tnl
          @tnl = tnl - exp_gained
          break
        else
          exp_gained = exp_gained - tnl
          gain_level
          return if self.is_a?(Avatar) && level >= HERO
        end
      end
    end

    ###########################################################################
    def gain_level
      # Calculate gains
      new_level      = level + 1
      new_tnl        = reset_tnl
      health_boost   = health_gain
      mana_boost     = mana_gain
      trains_boost   = trains_gain
      # Apply gains
      @level                   = new_level
      @tnl                     = (is_a?(Avatar) && hero?) ? 0 : new_tnl
      @training_points         += trains_boost
      health.max_stat.baseline += health_boost
      mana.max_stat.baseline   += mana_boost
      # heal
      heal
      # notify
      if is_a? Avatar
        puts "\n\nCONGRATULATIONS! You grow to level {G%u{x!" % level
        puts 'You gain {G%u{x health points.' % health_boost
        puts 'You gain {C%u{x mana points.' % mana_boost
        puts 'You gain {B%u{x training points.' % trains_boost
        puts 'You have {Y%u{x experience to your next level.' % tnl if !hero?
        Symphony.logger.debug '%s has grown to level %u with %u tnl' % [fqid_str, level, tnl]
      end
    end

    ###########################################################################
    def health_gain
      gain = level * (constitution.percent ** 2.0)
      Util::Computation.bound gain, level, MAX_HEALTH_GAIN
    end

    ###########################################################################
    def mana_gain
      gain = level * (focus.percent ** 2.0)
      Util::Computation.bound gain, level, MAX_MANA_GAIN
    end

    ###########################################################################
    def trains_gain
      1
    end

    ###########################################################################
    def reset_tnl
      # Force an insane amount of exp to reach final hero level.
      # TODO: Is this sound theory? If so, is this implementation reasonable?
      if level == HERO - 2
        TNL_FOR_HERO
      # Otherwise, TNL is an exponential function with respect to level.
      else
        level * (TNL_GROWTH ** level) * Random.rand(98..102)
      end
    end

    ###########################################################################
    def heal
      health.restore
      mana.restore
      stamina.restore
    end

    ###########################################################################
    def take_damage(amount, iDoesDamage)
      stamina.decrease 1
      health.decrease amount
      if health.current < 1
        die iDoesDamage
      end
    end

    ###########################################################################
    def die(iDoesDamage)
      result = self.run_hook :before_death
      return if result.halted?

      self.run_hook :on_death

      Symphony.logger.info '%s killed %s' % [iDoesDamage.whodunnit, self].map(&:fqid_str) if self.is_a?(Avatar)
      puts '%s {RKILLED{x you{x!!!!' % iDoesDamage.whodunnit.name
      puts 'You {RDIED{x!!!!'
      room.puts '$being is {RDEAD{x!!!!', self, exclude: self

      self.run_hook :after_death
    end

    ###########################################################################
    def announce_status
      str = "$being #{Display::Format.health(health.percent, true)}."
      room.puts str, self, exclude: self
    end

    ###########################################################################
    def circumstance_name
      if battle then 'battle'
      else 'normal'
      end
    end

    ###########################################################################
    # NOTE: Do we want to modify any stats based on height and weight?
    attr_writer :height, :weight

    ###########################################################################
    def modify(modifiers, reverse = false)
      modifiers.each do |stat_name, magnitude|
        stat      = send(stat_name)
        amount    = magnitude * (reverse ? -1 : 1)

        case stat
        when Mechanics::SimpleStat    then stat.modify amount
        when Mechanics::MaxStat       then stat.max_stat.modify amount
        when Mechanics::ThresholdStat then stat.current_stat.modify amount
        else raise ArgumentError.new "Being#modify given an unrecognized Stat type (#{send(stat).class.name})"
        end
      end
    end

    ###########################################################################
    def speak(content)
      puts I18n.t('being.speak.actor'), content
      room.puts I18n.t('being.speak.hear'), self, content, exclude: self
    end
  end
end

