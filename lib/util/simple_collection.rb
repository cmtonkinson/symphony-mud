# Define these classes together because otherwise autoload logic doesn't necessarily require the
# ADT before the concrete subclasses.
module Util

  ##################################################################################################
  # ADT to keep simple collections DRY
  ##################################################################################################
  class SimpleCollection

    def initialize(*args)
      @array = Array.[](*args)
    end

    def push(element)
      raise NoMethodError.new "Method not defined from ADT"
    end

    def pop
      return false if self.empty?
      @array.shift
    end

    def peek
      return false if self.empty?
      @array[0]
    end

    def empty?
      @array.empty?
    end

  end

  ##################################################################################################
  # Queue.
  ##################################################################################################
  class Queue < SimpleCollection

    def push(element)
      @array.push element
      self
    end

  end

  ##################################################################################################
  # Stack
  ##################################################################################################
  class Stack < SimpleCollection

    def push(element)
      @array.unshift element
      self
    end

  end

end
