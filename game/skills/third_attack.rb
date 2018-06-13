module Talent
  class ThirdAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :third_attack
    end

    def _added_to(being)
      skill = self
      being.after_strike_attack -> (strike) { skill.invoke strike }
    end
    
    def _invoke(strike)
      # Third attack Strikes only ever follow a second attack Strike.
      return unless strike.created_by == :second_attack

      # Generate a new Strike object, passing through all the details and
      # setting the first strike as the parent.
      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, :third_attack
    end

  end
end

