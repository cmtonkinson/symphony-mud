# Define these classes together because otherwise autoload logic doesn't necessarily require the
# ADT before the concrete subclasses.
module Util
  #############################################################################
  # ADT to keep simple collections DRY
  #############################################################################
  class SimpleCollection
    def initialize(*args)
      @array = Array.[](*args)
    end

    def push(_element)
      raise NoMethodError, 'Method not defined from ADT'
    end

    def pop
      return false if empty?

      @array.shift
    end

    def peek
      return false if empty?

      @array[0]
    end

    def empty?
      @array.empty?
    end

    def each(&block)
      @array.each(&block)
    end
  end

  #############################################################################
  # Queue.
  #############################################################################
  class Queue < SimpleCollection
    def push(element)
      @array.push element
      self
    end
  end

  #############################################################################
  # Stack
  #############################################################################
  class Stack < SimpleCollection
    def push(element)
      @array.unshift element
      self
    end
  end
end
