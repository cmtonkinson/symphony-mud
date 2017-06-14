module Mechanics

  class MaxStat

    attr_reader :current
    attr_accessor :max_stat

    ###########################################################################
    def initialize(max_value)
      @current  = max_value
      @max_stat = SimpleStat.new max_value
    end

    ###########################################################################
    def current=(value)
      @current = Util.bound value, 0, max
    end

    ###########################################################################
    def max
      @max_stat.current
    end

    ###########################################################################
    def percent
      1.0 * current / max
    end

    ###########################################################################
    def restore
      @current = max
    end

    ###########################################################################
    def increase(value)
      self.current = @current + value
    end

    ###########################################################################
    def decrease(value)
      self.current = @current - value
    end

  end

end
