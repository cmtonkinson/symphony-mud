require "pqueue"

module Mechanics
  class Schedule

    ###########################################################################
    def initialize
      @docket = PQueue.new { |a,b| a.wen < b.wen }
      @recurring = {}
    end

    ###########################################################################
    # Insert the Job into the priority queue.
    def add(job)
      @docket.push job
      Symphony.logger.debug "Scheduled %s" % job
    end

    ###########################################################################
    # Execute the next valid Job.
    #
    # Returns true if a Job was fired.
    # Returns false if a dead Job was pruned.
    # Returns nil if there was nothing to be done.
    def fire!
      return nil if @docket.empty?

      top_job = @docket.top

      if top_job.dead?
        @docket.pop
        return false
      end

      return nil unless top_job.ready?

      top_job.fire
      @docket.pop

      if top_job.recurring?
        again = top_job.recur!
        add again unless again.nil?
      end

      true
    end

    ###########################################################################
    def length
      @docket.length
    end

    ###########################################################################
    def take(n)
      @docket.to_a.reverse.take n
    end

  end
end
