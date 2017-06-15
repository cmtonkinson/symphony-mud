module Combat

  class Bout

    attr_reader :battle
    attr_reader :strikes

    ###########################################################################
    def initialize(attacker, defender, battle = nil)
      @battle  = battle || Battle.new
      @strikes = []
      Symphony.logger.warn "bout"
    end

    ###########################################################################
    def push(strike)
      @strikes << strike
    end

  end

end

