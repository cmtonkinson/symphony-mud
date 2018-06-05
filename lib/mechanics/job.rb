module Mechanics
  class Job
    # Who "owns" the Job? (this will be an Objekt object).
    attr_reader :who
    # What is to be done? (this will respond to #call)
    attr_reader :what
    # Optional arguments to be sent when :what is invoked
    attr_reader :args
    # When is the Job to be done? (unix time; integral seconds after epoch)
    attr_reader :wen

    attr_reader :repeat_every, :repeat_times

    ###########################################################################
    def initialize(wen, who, callable = nil, *args, &block)
      raise ArgumentError.new('no method or proc given') if callable.nil? && !block_given?
      raise ArgumentError.new('both method and proc given') if !callable.nil? && block_given?

      if !callable.nil? && !callable.respond_to?(:to_sym) && !callable.respond_to?(:call)
        raise ArgumentError.new('callable does not respond to :to_sym or :call')
      end

      # If the value of wen is less than 31,536,000 (in unix time that's
      # approximately Jan 1, 1971 or 1 year past epoch) then we assume it
      # represents a number of seconds from "now," rather than an absolute
      # time (which for any value under about 1.4 billion is in the past).
      @wen = wen
      @wen += Time.now.to_i if wen < 31_536_000

      # If the Job is being created for an Objekt, we want to make sure the
      # Objekt knows about the Job so it can bookkeep correctly.
      @who = who
      @who.jobs.add self if @who.is_a? Realm::Objekt

      # Either we have a block, or `callable` is a method name or something
      # responding to :call.
      @what = block_given? ? block : callable
      @args = args

      # Never felt more alive!
      @dead = false

      # One-off by default.
      @recurring    = false
      @repeat_every = 0
      @repeat_times = 0
    end

    ###########################################################################
    def recurring?
      @recurring
    end

    ###########################################################################
    def recur(every = 60, times = -1)
      @recurring    = true
      @repeat_every = every
      @repeat_times = times
    end

    ###########################################################################
    def recur!
      @wen = Time.now.to_i + @repeat_every
      if @repeat_times == 0
        return nil
      else
        return self
      end
    end

    ###########################################################################
    # Is it time for the Job to fire?
    def ready?(now = Time.now.to_i)
      now >= @wen
    end

    ###########################################################################
    # Execute the callable.
    def fire
      Symphony.logger.trace 'Firing %s' % self
      @repeat_times -= 1 if @repeat_times > 0
      @what.respond_to?(:call) ? @what.call(@who, *@args) : @who.send(@what.to_sym, *@args)
      kill unless recurring?
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
      return if dead?
      Symphony.logger.trace 'Killing %s' % self
      @who.jobs.delete self if @who.is_a? Realm::Objekt
      @who  = nil
      @what = nil
      @args = nil
      @dead = true
    end

    ###########################################################################
    # Has the Job been killed?
    def dead?
      @dead
    end

    ###########################################################################
    def ready_in
      @wen - Time.now.to_i
    end

    ###########################################################################
    def to_s
      dead? ? '<Dead Job>' : '<Job %s %s %s %s>' % [wen_to_s, who_to_s, what_to_s, args_to_s]
    end

    #############################################################################
    def wen_to_s
      '@%d (in %d)' % [wen, ready_in]
    end

    #############################################################################
    def who_to_s
      if @who.is_a? Realm::Objekt
        @who.fqid_str
      elsif !(@who.instance_of? Object)
        @who.to_s
      else
        @who.class.name
      end
    end

    #############################################################################
    def what_to_s
      if @what.respond_to? :to_sym
        ':' + @what.to_sym.to_s
      else
        '%s %s:%s' % [(@what.lambda? ? 'lambda' : 'proc'), *@what.source_location]
      end
    end

    #############################################################################
    def args_to_s
      if @args.length
        '(%s)' % @args.map do |arg|
          if arg.is_a? Realm::Objekt
            arg.fqid_str
          elsif arg.respond_to? :to_s
            arg.to_s
          else
            '?'
          end
        end.join(', ')
      else
        ''
      end 
    end
 
  end

  # RecurringJob provides a one-liner initialization for creating repeat Jobs.
  class RecurringJob < Job
    def initialize(wen, who, every, callable = nil, *args, &block)
      super wen, who, callable, *args, &block
      recur every, -1
    end
  end

end

