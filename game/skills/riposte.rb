module Talent
  class RiposteSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :riposte
    end

    def _learn
      skill = self
      able_being.after_ability_invocation do |ability_name, retval|
        if ability_name == :parry && retval == true
          skill.invoke
        end
      end
    end

    def _invoke
      return unless rand(1..100) < successRate

      # IMPORTANT ASSUMPTION:
      # If this is a riposte, we assume it immediately follows a
      # parry, which was the means of avoiding a Strike. We assume
      # that specific Strike instance is the last recorded Strike
      # for the current Bout. With that assumption, we can retrieve
      # context for who is to Strike who here.
      #
      # TODO: Fix this. It's really icky.
      current_bout = able_being.battle.bouts.last
      parent_strike = current_bout.strikes.last
      attacker = parent_strike.defender
      defender = parent_strike.attacker

      Combat::Strike.new attacker, defender, current_bout, parent_strike, name
    end
  end
end
