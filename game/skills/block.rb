module Talent
  class BlockSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super(*args)
      @category = :skill
      @name     = :block
    end

    def _learn
      skill = self
      able_being.before_strike_defense ->(strike) { skill.invoke strike unless strike.avoided }
    end

    def _invoke(strike)
      return unless rand(1..100) < successRate

      strike.avoid
      strike.defender.indented_puts strike.level, "You block $being's $strike!", strike.attacker, strike
      strike.attacker.indented_puts strike.level, '$being blocks your $strike!', strike.defender, strike
    end
  end
end
