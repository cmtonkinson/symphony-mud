module Talent
  class SecondAttackSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :second_attack
    end

    def added_to(being)
      skill = self
      being.after_strike_attack -> (strike) { skill.invoke strike }
    end

    def _invoke(strike)
      # Only the initial attack Strike during a normal Bout is subject to
      # being followed up by a second attack Strike.
      return unless strike.created_by == :bout

      # Generate a new Strike object, passing through all the details and
      # setting the first strike as the parent.
      Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike, :second_attack
    end

  end
end

