module Talent
  class SecondAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super(*args)
      @category = :skill
      @name     = :second_attack
    end

    def _learn
      skill = self
      able_being.after_strike_attack ->(strike) { skill.invoke strike if strike.created_by == :bout }
    end

    def _invoke(strike)
      return unless rand(1..100) < successRate

      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, name
    end
  end
end
