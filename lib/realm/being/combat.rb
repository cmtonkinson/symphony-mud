module Realm

  class Being
  
    define_hooks :before_strike_attack, :after_strike_attack
    define_hooks :before_strike_defense, :after_strike_defense
    define_hook :before_death, halts_on_falsey: true
    define_hook :on_death
    define_hook :after_death

    ###########################################################################
    def in_battle?
      @circumstance.is_a? Combat::Battle
    end

    ###########################################################################
    def peace
      @circumstance = nil
      scheduled_attacks = @jobs.select { |j| j.who == Combat::Strike }
      scheduled_attacks.each &:kill
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
      !hero?
    }

    ###########################################################################
    on_death -> {
      peace
    }

    ###########################################################################
    after_death -> {
    }

  end

end

