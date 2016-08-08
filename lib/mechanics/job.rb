module Mechanics
  class Job

    # Who "owns" the Job? (this will be an Objekt object).
    attr_reader :who
    # What is to be done? (this will respond to #call)
    attr_reader :what
    # When is the Job to be done? (unix time; integral seconds after epoch)
    attr_reader :wen

    ###########################################################################
    def initialize(wen, who, callable = nil, &block)
      raise ArgumentError.new("no method or proc given") if callable.nil? && !block_given?
      raise ArgumentError.new("both method and proc given") if !callable.nil? && block_given?

      if !callable.nil? && !callable.respond_to?(:to_sym) && !callable.respond_to?(:call)
        raise ArgumentError.new("callable does not respond to :to_sym or :call")
      end

      @wen  = wen
      @who  = who
      @what = block_given? ? block : callable
      @dead = false
    end

    ###########################################################################
    # Is it time for the Job to fire?
    def ready?(now = Time.now.to_i)
      now >= @wen
    end

    ###########################################################################
    # Execute the callable.
    def fire
      @what.respond_to?(:call) ? @what.call(@who) : @who.send(@what.to_sym)
    end

    ###########################################################################
    # Render the Job functionally useless. We make a time-space tradeoff here
    # by marking the Job dead rather than immediately removing it from the
    # Schedule's priority queue because skipping over a dead Job during
    # Schedule#fire is an O(1) operation, whereas to eagerly remove this Job
    # from the Schedule when it's killed would require iteration over the
    # priority queue, which is O(n). Since Jobs are small objects in memory,
    # this tradeoff is worth it.
    def kill
      @who = @what = @wen = nil
      @dead = true
    end

    ###########################################################################
    # Has the Job been killed?
    def dead?
      @dead
    end

  end
end
