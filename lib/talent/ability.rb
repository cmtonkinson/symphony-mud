module Talent
  class Ability
    # TODO: - some kind of global registry of available Abilities that can
    # be iterated, processed, etc. (esp. as to allow gods easy path toward
    # learning everything there is to be learned) perhaps with a static
    # Array and using self.inherited

    MIN_PRACTICE_BOOST = 5
    MAX_PRACTICE_LEVEL = 70

    attr_reader :type, :category, :name
    attr_reader :level, :trains, :difficulty, :stamina, :mana, :success_rate
    attr_reader :prerequisites
    attr_reader :able_being, :mastery

    ###########################################################################
    def initialize(level, trains, difficulty, stamina, mana, success_rate)
      @type          = nil # active or passive
      @category      = nil # skill, spell, craft, etc.
      @name          = nil
      @prerequisites = []

      @level         = level
      @trains        = trains
      @difficulty    = difficulty
      @stamina       = stamina
      @mana          = mana
      @success_rate  = success_rate

      @able_being    = nil
      @mastery       = 0 # 0 means unlearned
    end

    ###########################################################################
    def add_prerequisite(prereq)
      @prerequisites.push prereq
    end

    ###########################################################################
    def category_s
      category.to_s.tr '_', ' '
    end

    ###########################################################################
    def name_s
      name.to_s.tr '_', ' '
    end

    ###########################################################################
    def passive?
      type == :passive
    end

    ###########################################################################
    def active?
      type == :active
    end

    ###########################################################################
    def added_to(being)
      @able_being = being
    end

    ###########################################################################
    def learn(default_mastery = 1)
      @mastery = default_mastery
      _learn
    end

    ###########################################################################
    def learned?
      mastery > 0
    end

    ###########################################################################
    def can_learn?
      # You need to be a high enough level.
      return false if level > able_being.level

      # You need to have learned any and all prerequisite Abilities first.
      return false if prerequisites.any? { |prereq| !prereq.learned? }

      # I mean... I *guess* it's okay...
      true
    end

    ###########################################################################
    def prereq_string
      prerequisites.map(&:name_s).join(', ')
    end

    ###########################################################################
    def mastered?
      mastery == 100
    end

    ###########################################################################
    def master
      @mastery = 100
    end

    ###########################################################################
    # We only want to be able to practice an Ability up to 70%.
    #
    # The formula for how much mastery incresaes on each practice is roughly:
    #   y = 5.75*(6-d)+6
    # where `d` is the difficulty of the Ability. That means a difficulty 1
    # Ability increases by about 35 points per practices, whereas a difficulty
    # 5 Ability increases by about 12. Then we add a smidge of randomness so
    # it's not super boring.
    def practice
      return if mastery >= MAX_PRACTICE_LEVEL

      # The mastery gained on each practice is inversely proportional to the
      # difficulty. The formula here yields roughly 35 points per practice for
      # a difficulty 1 ability and about 12 points for a difficulty 5 ability.
      boost = 5.75 * (6.0 - difficulty) + 6.0

      # Add a 20% fudge factor.
      adjust = Random.rand(80..120) / 100
      boost *= adjust

      # Make sure they get SOMEthing, yet also don't go over the limit,
      max_boost = MAX_PRACTICE_LEVEL - mastery
      boost = Util::Computation.bound(boost, MIN_PRACTICE_BOOST, max_boost)

      @mastery += boost
    end

    ###########################################################################
    def improve(context = nil)
      if learned? && !mastered?
        @mastery += 1
        which = case context
                when nil then 'improve'
                when true then 'improve_success'
                when false then 'improve_failure'
                else Util.argerr "context must be nil, true, or false (#{context.class.name} given)"
                end
        able_being.puts I18n.t("talent.ability.#{which}"), name_s
      end
    end

    ###########################################################################
    # Conditionally improves on the Ability, based inversely on the difficulty.
    def randomly_improve(context = nil)
      improve context if Util::Computation.percent_chance 90 / difficulty
    end

    ###########################################################################
    def invoke(*args)
      # Do they have the resources to perform?
      able_being.puts I18n.t('talent.ability.err_no_stamina') and return if able_being.stamina.current < stamina
      able_being.puts I18n.t('talent.ability.err_no_mana') and return if able_being.mana.current < mana

      # Deduct resources.
      able_being.stamina.decrease stamina if stamina > 0
      able_being.mana.decrease mana if mana > 0

      # There is a random chance any non-mastered ability will fail.
      if Util::Computation.percent_chance(100 - mastery)
        able_being.puts I18n.t('talent.ability.confused'), name_s unless passive?
        # Randomly improve upon the ability. The more advanced you are, the
        # more instructive failure is.
        randomly_improve false if Util::Computation.percent_chance(mastery)
        return false
      end

      # NOTE: Here WOULD go a check for success_rate. Instead, because some
      # abilities need to handle failure in a unique way, that check must be
      # done per _invoke, for each sub-Ability.

      # Do the deed.
      able_being.run_hook :before_ability_invocation, name
      retval = _invoke(*args)
      able_being.run_hook :after_ability_invocation, name, retval

      # Randomly improve upon the ability. The more novice you are, the more
      # instructive success is.
      randomly_improve true if retval == true && Util::Computation.percent_chance(100 - mastery)

      retval
    end

    ###########################################################################
    def exec
      raise StandardError.new 'Talent::Ability#exec not implemented'
    end
  end
end
