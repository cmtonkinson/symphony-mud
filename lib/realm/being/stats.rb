module Realm
  class Being

    BASE_HEALTH = 100
    BASE_MANA   = 100
    MAX_STAMINA = 100

    BASE_LEVEL = 1
    BASE_EXP   = 0
    BASE_TNL   = 100
    TARGET_TNL = 100_00

    NEWBIE  = 1
    NOVICE  = 10
    HERO    = 100
    LORD    = HERO + 1
    DUKE    = HERO + 3
    KING    = HERO + 5
    DEMIGOD = HERO + 10
    GOD     = HERO + 15
    CREATOR = HERO + 20

    LEVELS = %w(NEWBIE NOVICE HERO LORD DUKE KING DEMIGOD GOD CREATOR)

    # Many immortal commands (such as 'force' and 'summon') require that in
    # order to perform them, the actor be a sufficiently higher level than the
    # victim. This is a sort of "chain of command" security measure and this is
    # minimum required level difference is codified as IMMORTAL_SENIORITY.
    IMMORTAL_SENIORITY = 5

    FITNESS_BASE      = 15
    FITNESS_THRESHOLD = 20

    DEFENSE_BASE = 0
    OFFENSE_BASE = 0

    # Core names can be grouped into five distinct buckets.
    EXPERIENCE = %i(level exp tnl)
    WELLNESS   = %i(health mana stamina)
    FITNESS    = %i(strength dexterity constitution intelligence focus creativity charisma luck)
    DEFENSE    = %i(armor bash slash pierce exotic)
    OFFENSE    = %i(hit_bonus damage_bonus)

    # Discourage direct manipulation of any core names.
    EXPERIENCE.each { |name| attr_reader name }
    WELLNESS.each { |name| attr_reader name }
    FITNESS.each { |name| attr_reader name }
    DEFENSE.each { |name| attr_reader name }
    OFFENSE.each { |name| attr_reader name }

    # Shortcuts!
    FITNESS.each { |name| alias_method :"#{name[0..2]}", :"#{name}" }

    attr_accessor :circumstance

    ###########################################################################
    # Set default values for performance attributes.
    after_initialize -> {
      @level = BASE_LEVEL
      @exp   = BASE_EXP
      @tnl   = BASE_TNL

      @health  = Mechanics::MaxStat.new BASE_HEALTH
      @mana    = Mechanics::MaxStat.new BASE_MANA
      @stamina = Mechanics::MaxStat.new MAX_STAMINA

      FITNESS.each { |name| instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(FITNESS_BASE, FITNESS_THRESHOLD) }
      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(DEFENSE_BASE) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(OFFENSE_BASE) }

      @circumstance = nil
    }

    ###########################################################################
    on_hashify -> {
      hash = Hash.new

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

      {stats: hash}
    }

    ###########################################################################
    on_dehashify -> hash {
      hash = hash[:stats]

      EXPERIENCE.each { |name| instance_variable_set "@#{name}", hash[name] }

      WELLNESS.each do |name|
        cur, max = hash[name].split "/"
        instance_variable_set "@#{name}", Mechanics::MaxStat.new(max.to_i)
        send(name).current = cur.to_i
      end

      FITNESS.each do |name|
        baseline, threshold = hash[name].split ":"
        instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(baseline.to_i, threshold.to_i)
      end

      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }
    }

    ###########################################################################
    vivify -> {
      $world.schedule Mechanics::RecurringJob.new 0, self, 60, :auto_health
      $world.schedule Mechanics::RecurringJob.new 0, self, 60, :auto_mana
      $world.schedule Mechanics::RecurringJob.new 0, self, 3, :auto_stamina
    }

    ###########################################################################
    def newbie?
      @level < NOVICE
    end

    ###########################################################################
    (LEVELS - %w(NEWBIE)).each do |lvl|
      define_method :"#{lvl.downcase}?", Proc.new { @level >= Realm::Being.const_get(lvl) }
    end

    ###########################################################################
    def has_seniority?(victim)
      victim.level < @level - IMMORTAL_SENIORITY
    end

    ###########################################################################
    def level_label
      LEVELS.reverse.find { |lvl| @level >= Realm::Being.const_get(lvl) }.downcase
    end

    ###########################################################################
    def auto_health
      this_heal = Util.bound health.max / 10, 1, health.current
      health.increase this_heal
    end

    ###########################################################################
    def auto_mana
      this_heal = Util.bound mana.max / 10, 1, mana.current
      mana.increase this_heal
    end

    ###########################################################################
    def auto_stamina
      this_heal = Util.bound level / 5, 1, MAX_STAMINA / 2.5
      stamina.increase this_heal
    end

    ###########################################################################
    # NOTE: Avatars stop gaining experience and show 0 tnl once they reach HERO
    def award_experience(exp_gained)
      return if self.is_a?(Avatar) && level >= HERO

      puts "You gain {Y%d{x experience points!" % exp_gained
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
      new_level    = level + 1
      new_tnl      = 1
      health_boost = 1
      mana_boost   = 1
      trains_boost = 1
      # Apply gains
      @level = new_level
      @tnl   = (is_a?(Avatar) && hero?) ? 0 : new_tnl
      health.max_stat.baseline += health_boost
      # heal
      heal
      # notify
      puts "\n\nCONGRATULATIONS! You grow to level {G%u{x!" % level
      puts "You gain {G%u{x health points." % health_boost
      puts "You gain {C%u{x mana points." % mana_boost
      puts "You gain {B%u{x training points." % trains_boost
      puts "You have {Y%u{x experience to your next level." % tnl if (is_a?(Avatar) && hero?)
      Symphony.logger.debug "%s has grown to level %u with %u tnl" % [fqid_str, level, tnl]
    end

    ###########################################################################
    def heal
      health.restore
      mana.restore
      stamina.restore
    end

    ###########################################################################
    def take_damage(amount, iDoesDamage)
      Symphony.logger.error amount
      Symphony.logger.error health.current
      stamina.decrease 1
      health.decrease amount
      Symphony.logger.error health.current
      die(iDoesDamage) if health.current == 0
    end

    ###########################################################################
    def die(iDoesDamage)
      result = self.run_hook :before_death
      return if result.halted?

      self.run_hook :on_death

      Symphony.logger.info "%s killed %s" % [iDoesDamage.whodunnit, self].map(&:fqid_str) if self.is_a?(Avatar)
      puts "%s {RKILLED{X you{x!!!!" % iDoesDamage.whodunnit.name
      puts "You {RDIED{x!!!!"
      room.puts "$being is {RDEAD{x!!!!", self, exclude: self
    end

    ###########################################################################
    def announce_status
      p = health.percent

      str = if p > 0.98 then "$being looks {Gfantastic{x."
      elsif    p > 0.90 then "$being is in {Bexcellent{x condition."
      elsif    p > 0.70 then "$being is in {Mgreat{x condition."
      elsif    p > 0.50 then "$being is in {Cgood{x condition."
      elsif    p > 0.35 then "$being looks a little {cworn{x."
      elsif    p > 0.20 then "$being looks pretty {mworn{x."
      elsif    p > 0.10 then "$being looks very {ybattered{x."
      elsif    p > 0.05 then "$being is pretty {rbeat up{x."
      else                   "$being looks {wterrible{x."
      end

      room.puts str, self, exclude: self
    end

    ###########################################################################
    def circumstance_name
      case @circumstance
      when Combat::Battle then "battle"
      else "normal"
      end
    end

  end
end
