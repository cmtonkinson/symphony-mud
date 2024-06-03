module Talent
  class ThirdAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super
      @category = :skill
      @name     = :third_attack
    end

    def _learn
      skill = self
      able_being.after_strike_attack ->(strike) { skill.invoke strike if strike.created_by == :second_attack }
    end

    def _invoke(strike)
      return unless rand(1..100) < success_rate

      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, name
    end
  end
end
