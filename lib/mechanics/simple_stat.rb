module Mechanics

  class SimpleStat

    attr_accessor :baseline, :modifier

    ###########################################################################
    def initialize(baseline = 0)
      @baseline = baseline
      @modifier = 0
    end

    ###########################################################################
    def current
      true_modified = @baseline + @modifier
      true_modified < 1 ? 1 : true_modified
    end

    ###########################################################################
    def modify(magnitude)
      @modifier += magnitude
    end

    ###########################################################################
    def unmodify(magnitude)
      @modifier -= magnitude
    end

  end

end
