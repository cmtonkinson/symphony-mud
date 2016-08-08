module Realm
  class World < Objekt

    ORIGIN_ZONE_ID = "00000000-0000-4000-a000-000000000000"
    THE_VOID_ID    = "00000000-0000-4000-a000-000000000001"

    JOBS_PER_BATCH = 5

    CONTAINERS = %w(zones)

    CONTAINERS.each { |cont| attr_accessor cont }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
      @schedule = Mechanics::Schedule.new
      schedule 2, self, -> world { Symphony.logger.absurd "TICK!!!" }

      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    def load_zones
      Realm::Scribe.the_story_of_baron_munchausen.each do |zone_id|
        @zones << Realm::Scribe.load(Realm::Zone, zone_id)
      end
    end

    ###########################################################################
    def find(objekt_id)
      Util::Registry.lookup objekt_id
    end
    alias_method :[], :find

    ###########################################################################
    def find_room(room_id)
      (room = find room_id) ? room : find(THE_VOID_ID)
    end

    ###########################################################################
    def schedule(*args, &block)
      @schedule.add Mechanics::Job.new *args, &block
    end

    ###########################################################################
    # Execute JOBS_PER_BATCH Jobs' worth of actual useful work.
    def handle_jobs
      ctr = 0
      while ctr < JOBS_PER_BATCH
        # Try to fire the next Job.
        retval = @schedule.fire!

        # Stop processing if there is nothing more to be done.
        break if retval == nil

        # Don't count pruning a dead Job as doing actual work.
        next if retval == false

        ctr += 1
      end
    end

  end
end
