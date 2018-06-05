module Logistics
  class Pathfinder
    attr_reader :start, :finish

    def initialize(start, finish)
      @start  = start
      @finish = finish
    end
  end
end

