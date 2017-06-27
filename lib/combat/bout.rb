module Combat

  class Bout

    attr_reader :battle
    attr_reader :strikes

    ###########################################################################
    def initialize(battle)
      @battle  = battle
      @strikes = []
      make_strikes 
    end

    ###########################################################################
    def push(strike)
      @strikes << strike
    end

    ###########################################################################
    # During each Bout, each belligerent attacks one defender. That attack
    # could be the first of of an entire back-and-forth (in light of e.g.
    # 'second attack' or 'counter attack' skills) but everybody gets at least
    # one shot in per Bout.
    def make_strikes
      # Don't start the Bout output on a line with a prompt. It's weird.
      battle.belligerents.each { |b| b.puts "" }

      battle.belligerents.each do |attacker|
        next if attacker.dead?

        defender = battle.opponents_of(attacker).first
        next if defender.dead?

        Strike.new attacker, defender, self        
      end
    end

  end

end

