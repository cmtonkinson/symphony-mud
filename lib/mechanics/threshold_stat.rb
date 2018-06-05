module Mechanics
  class ThresholdStat
    attr_reader :current_stat, :threshold

    ###########################################################################
    def initialize(baseline, threshold)
      @current_stat = SimpleStat.new baseline
      @threshold    = threshold
    end

    ###########################################################################
    def current
      @current_stat.current
    end

    ###########################################################################
    def percent
      1.0 * current / threshold
    end
  end
end
