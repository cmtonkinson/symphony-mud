require 'benchmark'
require 'pqueue'

module Mechanics
  class Schedule
    attr_reader :docket, :execution_times, :jobs_per_batch

    MIN_JOBS_PER_BATCH = 25

    ###########################################################################
    def initialize
      @docket             = PQueue.new { |a, b| a.wen < b.wen }
      @recurring          = {}
      @execution_times    = []
      @jobs_per_batch     = MIN_JOBS_PER_BATCH
      @job_cycle_counter  = 0
      @jobs_this_cycle    = 0
      @batches_per_second = 1 / Base.configuration.sleep_interval
    end

    ###########################################################################
    # Insert the Job into the priority queue.
    def add(job)
      @docket.push job
      Symphony.logger.trace 'Scheduling %s' % job unless Mechanics::Job::IGNORE_TRACE.call(job.to_s)
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

      realtime = Benchmark.realtime do
        top_job.fire
      end
      # Keep track of execution times (in microseconds)
      @execution_times << realtime * 1_000_000.0

      @docket.pop

      if top_job.recurring?
        again = top_job.recur!
        add again unless again.nil?
      end

      true
    end

    ###########################################################################
    # Execute JOBS_PER_BATCH Jobs' worth of actual useful work.
    def fire_batch
      ctr = 0
      while ctr < @jobs_per_batch
        # Try to fire the next Job.
        retval = fire!

        # Stop processing if there is nothing more to be done.
        break if retval == nil

        # Don't count pruning a dead Job as doing actual work.
        next if retval == false

        ctr += 1
        @jobs_this_cycle += 1
      end

      rebalance_job_parameters
    end

    ###########################################################################
    def rebalance_job_parameters
      # Don't run more than once per second.
      @job_cycle_counter += 1
      return unless @job_cycle_counter > @batches_per_second

      # Reset counters.
      Symphony.logger.trace 'World#rebalance_job_parameters'
      @job_cycle_counter = 0
      @jobs_this_cycle   = 0

      # Get the next job that needs to be run.
      queue = docket.send :que
      top = queue.last

      if top.overdue?
        # If the top job is overdue, we may be slipping on our schedule.
        # How many jobs delinquent will we be one second from now?
        behind_by = queue.count { |job| job.wen <= Time.now.to_i + 1 }

        # Figure out how many jobs we'd need to run per batch to catch up
        # over the next second.
        catch_up_rate = (behind_by / @batches_per_second).ceil

        # A decrease probably isn't wise at this point. That being said, we
        # entered this code section because we were delinquent on the top job.
        # As a safety measure (to make sure we're doing JUST better than
        # treading water), bump the rate slightly.
        if catch_up_rate < @jobs_per_batch
          catch_up_rate = (@jobs_per_batch * 1.1).ceil
        end

        # Increase jobs_per_batch.
        Symphony.logger.debug "World#rebalance_job_parameters \
          - increasing batch size from #{@jobs_per_batch} to #{catch_up_rate}"
        @jobs_per_batch = catch_up_rate

      elsif top.ready?
        # If the top job is ready, do nothing.
        # TODO - is this correct?
      else
        # If the top job isn't ready yet, we may have a higher batch size than
        # necessary. Reduce the batch size by 10%.
        new_rate = (@jobs_per_batch * 0.9).ceil
        return if new_rate < MIN_JOBS_PER_BATCH

        Symphony.logger.debug "World#rebalance_job_parameters - \
          decreasing batch size from #{@jobs_per_batch} to #{new_rate}"
        @jobs_per_batch = new_rate
      end
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
