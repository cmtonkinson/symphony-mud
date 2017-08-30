module Realm

  class Being

    attr_reader :group

    define_hooks :before_strike_attack, :after_strike_attack
    define_hooks :before_strike_defense, :after_strike_defense
    define_hook :before_death, halts_on_falsey: true
    define_hook :on_death
    define_hook :after_death

    ###########################################################################
    after_initialize -> {
      @dead  = false
      Combat::Group.new self
    }

    ###########################################################################
    enervate -> {
      # Ensure the Being is removed from Battle.
      @circumstance.delete_belligerent self if in_battle?
    }

    ###########################################################################
    def set_group(group)
      @group = group
    end

    ###########################################################################
    def battle
      group.battle
    end

    ###########################################################################
    def dead?
      @dead
    end

    ###########################################################################
    before_strike_attack -> (strike) {
    }

    ###########################################################################
    after_strike_attack -> (strike) {
      # TODO Leaving this here as a reminder of how to implement e.g. Second Strike
      #return if strike.parent
      #Combat::Strike.new strike.attacker, strike.defender, strike.bout, strike
    }

    ###########################################################################
    before_strike_defense -> (strike) {
    }

    ###########################################################################
    after_strike_defense -> (strike) {
    }

    ###########################################################################
    before_death -> {
      return false if god?
      return true
    }

    ###########################################################################
    on_death -> {
      @dead = true
      group.run_hook :on_death, self
    }

  end

end

