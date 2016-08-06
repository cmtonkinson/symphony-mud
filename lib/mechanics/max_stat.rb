module Mechanics

  class MaxStat

    attr_reader :current
    attr_accessor :max_stat

    ###########################################################################
    def initialize(max_value)
      @current  = 0
      @max_stat = SimpleStat.new max_value
    end

    ###########################################################################
    def current=(value)
      @current = self.class.bound value, 0, max
    end

    ###########################################################################
    def max
      @max_stat.current
    end

    ###########################################################################
    def self.bound(x, low, high)
      x < low ? (low) : (x > high ? high : x)
    end

  end

end
