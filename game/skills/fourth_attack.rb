module Talent
  class FourthAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category   = :skill
      @name       = :fourth_attack
    end

    def _added_to(being)
      skill = self
      being.after_strike_attack -> (strike) { skill.invoke strike if strike.created_by == :third_attack }
    end
    
    def _invoke(strike)
      return unless rand(1..100) < successRate
      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, name
    end

  end
end

