module Combat


  class Battle

    attr_reader :belligerents

    def initialize
      Symphony.logger.warn "battle"
      @belligerents = Set.new
    end

    def add_belligerent(being)
      @belligerents.add being
    end

    def opponents_of(being)
      @belligerents.difference([being]).to_a
    end

  end

end

