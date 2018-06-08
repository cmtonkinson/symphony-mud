module Talent
  class BlockSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :block
    end

    def added_to(being)
      skill = self
      being.before_strike_defense -> (strike) { skill.invoke strike }
    end

    def _invoke(strike)
      raise "broken"
    end

  end
end
