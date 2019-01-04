module Talent
  class FourthAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super(*args)
      @category   = :skill
      @name       = :fourth_attack
    end

    def _learn
      skill = self
      able_being.after_strike_attack ->(strike) { skill.invoke strike if strike.created_by == :third_attack }
    end

    def _invoke(strike)
      return unless rand(1..100) < success_rate

      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, name
    end
  end
end
