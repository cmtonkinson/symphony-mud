module Realm
  class Being
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

    LEVELS = %w[NEWBIE TRAVELER HERO DEMIGOD GOD CREATOR].freeze

    # Many immortal commands (such as 'force' and 'summon') require that in
    # order to perform them, the actor be a sufficiently higher level than the
    # victim. This is a sort of "chain of command" security measure and this
    # minimum required level difference is codified as IMMORTAL_SENIORITY.
    IMMORTAL_SENIORITY = 5

    EXPERIENCE = %i[level exp tnl training_points].freeze

    EXPERIENCE.each { |name| attr_reader name }

    after_initialize lambda {
      @level           = BASE_LEVEL
      @exp             = BASE_EXP
      @tnl             = BASE_TNL
      @training_points = BASE_TRAINS
    }

    on_hashify lambda {
      hash = {}
      EXPERIENCE.each { |name| hash[name] = send name }
      { level: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:level]
      EXPERIENCE.each { |name| instance_variable_set "@#{name}", hash[name] }
    }

    def newbie?
      @level < TRAVELER
    end

    (LEVELS - %w[NEWBIE]).each do |lvl|
      define_method :"#{lvl.downcase}?", (proc { @level >= Realm::Being.const_get(lvl) })
    end

    def seniority?(victim)
      victim.level < @level - IMMORTAL_SENIORITY
    end

    def level_label
      LEVELS.reverse.find { |lvl| @level >= Realm::Being.const_get(lvl) }.downcase
    end

    # NOTE: Avatars stop gaining experience and show 0 tnl once they reach HERO
    def award_experience(exp_gained)
      return if is_a?(Avatar) && level >= HERO

      puts 'You gain {Y%d{x experience points!' % exp_gained
      @exp = exp + exp_gained

      while exp_gained.positive?
        if exp_gained < tnl
          @tnl = tnl - exp_gained
          break
        else
          exp_gained -= tnl
          gain_level
          return if is_a?(Avatar) && level >= HERO
        end
      end
    end

    def gain_level
      # Calculate gains
      new_level      = level + 1
      new_tnl        = reset_tnl
      health_boost   = health_gain
      mana_boost     = mana_gain
      trains_boost   = trains_gain
      # Apply gains
      @level = new_level
      @tnl = is_a?(Avatar) && hero? ? 0 : new_tnl
      @training_points         += trains_boost
      health.max_stat.baseline += health_boost
      mana.max_stat.baseline   += mana_boost
      # heal
      heal

      return unless is_a? Avatar

      # notify
      puts "\n\nCONGRATULATIONS! You grow to level {G%u{x!" % level
      puts 'You gain {G%u{x health points.' % health_boost
      puts 'You gain {C%u{x mana points.' % mana_boost
      puts 'You gain {B%u{x training points.' % trains_boost
      puts 'You have {Y%u{x experience to your next level.' % tnl unless hero?
      Symphony.logger.debug '%s has grown to level %u with %u tnl' % [fqid_str, level, tnl]
    end

    def health_gain
      gain = level * (constitution.percent**2.0)
      Util::Computation.bound gain, 1, MAX_HEALTH_GAIN
    end

    def mana_gain
      gain = level * (focus.percent**2.0)
      Util::Computation.bound gain, 1, MAX_MANA_GAIN
    end

    def trains_gain
      1
    end

    def reset_tnl
      # Force an insane amount of exp to reach final hero level.
      # TODO: Is this sound theory? If so, is this implementation reasonable?
      if level == HERO - 2
        TNL_FOR_HERO
      # Otherwise, TNL is an exponential function with respect to level.
      else
        level * (TNL_GROWTH**level) * Random.rand(98..102)
      end
    end
  end
end
