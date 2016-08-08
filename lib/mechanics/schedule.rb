require "pqueue"

module Mechanics
  class Schedule

    ###########################################################################
    def initialize
      @docket = PQueue.new { |a,b| a.wen < b.wen }
    end

    ###########################################################################
    # Insert the Job into the priority queue.
    def add(job)
      @docket.push job
    end
    alias_method :<<, :add

    ###########################################################################
    # Execute the next valid Job.
    #
    # Returns true if a Job was fired.
    # Returns false if a dead Job was pruned.
    # Returns nil if there was nothing to be done.
    def fire!
      return nil if @docket.empty?

      next_job = @docket.top

      if next_job.dead?
        @docket.pop
        return false
      end

      return nil unless next_job.ready?

      next_job.fire
      @docket.pop
      true
    end

  end
end
