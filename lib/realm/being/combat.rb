module Realm

  class Being
  
    define_hooks :before_strike_attack, :after_strike_attack
    define_hooks :before_strike_defense, :after_strike_defense
    define_hook :before_death, halts_on_falsey: true
    define_hook :on_death
    define_hook :after_death

    ###########################################################################
    after_initialize -> {
      @dead = false
    }

    ###########################################################################
    enervate -> {
      # Ensure the Being is removed from Battle.
      @circumstance.delete_belligerent self if in_battle?
    }

    ###########################################################################
    def in_battle?
      @circumstance.is_a? Combat::Battle
    end

    ###########################################################################
    def dead?
      @dead
    end

    ###########################################################################
    def peace
      @circumstance.run_hook :on_death, self if in_battle?

      unless dead?
        $world.schedule(Mechanics::Job.new(30, self) { @dead = false })
      end
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
      !god?
    }

    ###########################################################################
    on_death -> {
      @dead = true
      peace
    }

  end

end

