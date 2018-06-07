module Util
  class LimitStack < Stack

    def initialize(limit, *args)
      @limit = limit
      super(*args)
    end

    def push(element)
      @array.push element
      pop if @array.length > @limit 
      self
    end

    def limit
      @limit
    end

    def length
      @array.length
    end

  end
end
