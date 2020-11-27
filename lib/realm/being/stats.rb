module Realm
  class Being
    BASE_HEALTH     = 100
    MAX_HEALTH_GAIN = 10_000
    MAX_MANA_GAIN   = 10_000
    BASE_MANA       = 100
    MAX_STAMINA     = 100

    FITNESS_BASE      = 15
    FITNESS_THRESHOLD = 20

    DEFENSE_BASE = 0
    OFFENSE_BASE = 0

    BASE_AGE    = 17
    BASE_HEIGHT = 3
    BASE_WEIGHT = 3

    # Core names can be grouped into five distinct buckets.
    WELLNESS   = %i[health mana stamina].freeze
    FITNESS    = %i[strength dexterity constitution intelligence focus creativity charisma luck].freeze
    DEFENSE    = %i[armor bash slash pierce exotic].freeze
    OFFENSE    = %i[hit_bonus damage_bonus].freeze
    PHYSICAL   = %i[age height weight].freeze

    # Discourage direct manipulation of any core names by only defining readers.
    (WELLNESS + FITNESS + DEFENSE + OFFENSE + PHYSICAL).each do |name|
      attr_reader name
    end

    # Shortcuts!
    FITNESS.each { |name| alias_method :"#{name[0..2]}", :"#{name}" }

    # Random attributes of Being that can include static modifiers.
    MODIFYING_ATTRS = %w[klass anatomy].freeze

    attr_accessor :circumstance, :resistances

    ###########################################################################
    # Set default values for performance attributes.
    after_initialize lambda {
      @health  = Mechanics::MaxStat.new BASE_HEALTH
      @mana    = Mechanics::MaxStat.new BASE_MANA
      @stamina = Mechanics::MaxStat.new MAX_STAMINA

      # rubocop:disable Layout/LineLength
      FITNESS.each { |name| instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(FITNESS_BASE, FITNESS_THRESHOLD) }
      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(DEFENSE_BASE) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(OFFENSE_BASE) }
      # rubocop:enable Layout/LineLength

      @age    = BASE_AGE
      @height = BASE_HEIGHT
      @weight = BASE_WEIGHT

      @circumstance = nil
      @resistances = {}
    }

    ###########################################################################
    enervate lambda {
      @resistances = nil
    }

    ###########################################################################
    on_hashify lambda {
      hash = {}

      # NOTE: Must remove (and save) equipment before saving core stats. Not
      # only does this make it easier to prevent and detect errors with save
      # and restore logic, but it also makes it possible to edit Avatar files
      # (esp. stats and equipment) by hand without worrying about cascading
      # side effects.
      #
      # Unfortunately, this means we have to unload, save, and reload all
      # equipment during Being#on_hashify.
      saved_eq = {}
      if @anatomy
        hash[:equipment] = Hash[@anatomy.slots.map do |slot|
          next if slot.empty?

          item = slot.set nil
          saved_eq[slot.name] = item
          [slot.name, item.hashify]
        end.compact]
      end

      # NOTE: Same comments as above for equipment, except for the subobjects
      # defined in MODIFYING_ATTRS.
      MODIFYING_ATTRS.each do |x|
        x = send x.to_sym
        next if x.nil?

        modify x.modifiers, true if x.respond_to? :modifiers
      end

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
    on_dehashify lambda { |hash|
      hash = hash[:stats]

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

      PHYSICAL.each { |name| instance_variable_set "@#{name}", hash[name] if hash.key? name }

      # Must restore core stats before restoring equipment
      # See comments in on_hashify.
      # TODO: Deduplicate this logic with the [logically] equivalent loop at the end of on_hashify.
      if @anatomy && hash.key?(:equipment)
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
    vivify lambda {
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
    def heal
      health.restore
      mana.restore
      stamina.restore
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
      Symphony.logger.trace "Modifying #{fqid_str} with #{modifiers.inspect} (reverse = #{reverse})"
      modifiers.each do |name, magnitude|
        if Konst::ELEMENTS.include? name
          modify_resistance name, magnitude, reverse
        else
          modify_stat name, magnitude, reverse
        end
      end
    end

    ###########################################################################
    def modify_stat(name, magnitude, reverse = false)
      stat   = send(name)
      amount = magnitude * (reverse ? -1 : 1)

      case stat
      when Mechanics::SimpleStat    then stat.modify amount
      when Mechanics::MaxStat       then stat.max_stat.modify amount
      when Mechanics::ThresholdStat then stat.current_stat.modify amount
      else raise ArgumentError, "Being#modify given an unrecognized Stat type (#{send(stat).class.name})"
      end
    end

    ###########################################################################
    def modify_resistance(element, magnitude, reverse = false)
      amount = magnitude * (reverse ? -1 : 1)
      resistances[element] ||= Konst::BEING_RESIST_NEUTRAL
      resistances[element] += amount
      resistances.delete_if { |_, val| val == Konst::BEING_RESIST_NEUTRAL }
    end

    ###########################################################################
    def effective_resistance(element)
      return Konst::BEING_RESIST_NEUTRAL unless resistances.key? element

      Util::Computation.bound(resistances[element], Konst::BEING_RESIST_MIN, Konst::BEING_RESIST_MAX)
    end

    ###########################################################################
    def speak(content)
      puts I18n.t('being.speak.actor'), content
      room.puts I18n.t('being.speak.hear'), self, content, exclude: self
    end

    ###########################################################################
    # rubocop:disable Metrics/AbcSize
    def detail_plate
      # This ugly garbage is to try to keep the elemental resistance list in
      # the output flexible in the face of future changes or additions, without
      # a bunch of manual labor to maintain the plate template itself.
      # The first position of each pair is the `element:` label and the second
      # position is the user-friendly formatted output.
      r = []
      Konst::ELEMENTS.each do |el|
        res  = effective_resistance el
        pair = ["#{el}:", Display::Format.resistance_label(res)]
        r << pair
      end

      $templates.detail_plate % [
        level, str.current, hit_bonus.current, r[0][0], r[0][1],
        health.current, health.max, dex.current, damage_bonus.current, r[1][0], r[1][1],
        mana.current, mana.max, con.current, r[2][0], r[2][1],
        stamina.current, int.current, armor.current, r[3][0], r[3][1],
        foc.current, bash.current, r[4][0], r[4][1],
        exp, cre.current, slash.current, r[5][0], r[5][1],
        tnl, cha.current, pierce.current, r[6][0], r[6][1],
        training_points, luc.current, exotic.current, r[7][0], r[7][1]
      ]
    end
    # rubocop:enable Metrics/AbcSize
  end
end
