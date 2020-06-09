module Combat
  class Bout
    attr_reader :battle
    attr_reader :strikes

    ###########################################################################
    def initialize(battle)
      @battle  = battle
      @strikes = []
    end

    ###########################################################################
    def start
      make_strikes
    end

    ###########################################################################
    def push(strike)
      @strikes.push strike
    end

    ###########################################################################
    # During each Bout, each group group member attacks one defender.
    # That attack could be the first of of an entire back-and-forth (in light
    # of e.g. 'second attack' or 'counter attack' skills) but everybody gets at
    # least one shot in per Bout.
    def make_strikes
      fighters = battle.groups.map(&:members).reduce(:+)

      # Don't start the Bout output on a line with a prompt. It's weird.
      fighters.each { |f| f.puts '' }

      fighters.each do |attacker|
        next if attacker.dead?

        # TODO
        # This isn't terribly awesome. We use the ability of a fighter to
        # acquire a valid target as our criteria for continuing the Battle (see
        # Battle#new_bout) but unfortunately I don't see a clear way at the
        # time of writing to cache that acquired target for use here, since
        # conditions may have changed since it was computed.
        #
        # In the future, it would be nice to revisit this mechanic such that
        # we weren't performing the same non-trivial operation twice per Bout
        # per participant. For example, perhaps a "removed from active combat"
        # flag or hook could be implemented to help with this.
        defender = attacker.acquire_target
        next if defender.nil?

        Strike.new attacker, defender, self
        defender.announce_status unless defender.dead?
      end
    end
  end
end
