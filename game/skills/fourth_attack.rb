module Talent
  class FourthAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :fourth_attack
    end

    def added_to(being)
      skill = self
      being.after_strike_attack -> (strike) { skill.invoke strike }
    end
    
    def _invoke(strike)
      # Fourth attack Strikes only ever follows a third attack Strike.
      return unless strike.created_by == :third_attack

      # Generate a new Strike object, passing through all the details and
      # setting the first strike as the parent.
      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, :fourth_attack
    end

  end
end

