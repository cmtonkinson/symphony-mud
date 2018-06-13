module Talent
  class RiposteSkill < Ability
    include PassiveAbility

    def initialize(*args)
      super *args
      @category = :skill
      @name     = :riposte
    end

    def _added_to(being)
      skill = self
      being.after_ability_invocation -> (ability_name) { skill.invoke if ability_name == :parry }
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

      parent_strike.defender.puts '  You respond quickly with a riposte!'
      parent_strike.attacker.puts '  $being responds quickly with a riposte!', parent_strike.defender
      Combat::Strike.new parent_strike.defender, parent_strike.attacker, current_bout, parent_strike, name
    end

  end
end
