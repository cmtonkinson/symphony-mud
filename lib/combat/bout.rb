module Combat

  class Bout

    attr_reader :battle

    def initialize(attacker, defender, battle = nil)
      @battle = battle || Battle.new
      Symphony.logger.warn "bout"
    end
  end

end

