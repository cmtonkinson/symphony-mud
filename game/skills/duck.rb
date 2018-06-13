module Talent
  class DuckSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :duck
    end

    def _added_to(being)
      skill = self
      being.before_strike_defense -> (strike) { skill.invoke strike unless strike.avoided }
    end

    def _invoke(strike)
      return unless rand(1..100) < successRate

      strike.avoid
      strike.defender.puts "You duck away from $being's strike!", strike.attacker
      strike.attacker.puts '$being ducks away from your strike!', strike.defender
    end

  end
end
