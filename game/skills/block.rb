module Talent
  class BlockSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :block
    end

    def _added_to(being)
      skill = self
      being.before_strike_defense -> (strike) { skill.invoke strike unless strike.avoided }
    end

    def _invoke(strike)
      return unless rand(1..100) < successRate

      strike.avoid
      strike.defender.puts "You block $being's strike!", strike.attacker
      strike.attacker.puts '$being blocks your strike!', strike.defender
    end

  end
end
