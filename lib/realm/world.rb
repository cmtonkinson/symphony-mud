module Realm
  class World < Objekt
    ORIGIN_ZONE_ID = '00000000-0000-4000-a000-000000000000'
    THE_VOID_ID    = '00000000-0000-4000-a000-000000000001'

    CONTAINERS = %w[zones beings avatars]

    CONTAINERS.each { |cont| attr_accessor cont }

    ###########################################################################
    def initialize(id = nil, props = {})
      props[:name] ||= 'World'
      super
    end

    ###########################################################################
    after_initialize -> {
      @schedule = Mechanics::Schedule.new
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    def load_zones
      Logistics::Scribe.the_story_of_baron_munchausen.each do |zone_id|
        zone = Logistics::Scribe.load(Realm::Zone, zone_id)
        next unless zone
        @zones.insert zone
        zone.run_hook :vivify
      end
    end

    ###########################################################################
    def save
      Symphony.logger.info 'World#save beginning'
      s = Logistics::Scribe
      @zones.each { |id, z| s.save z }
      @avatars.each { |id, a| s.save a }
      Symphony.logger.info 'World#save complete'
    end

    ###########################################################################
    def find(objekt_id)
      Util::Registry.lookup objekt_id
    end
    alias_method :[], :find

    ###########################################################################
    def find_room(rid_or_uuid)
      return find THE_VOID_ID if !rid_or_uuid
      # It's a RID
      if rid_or_uuid.is_a? Numeric
        return nil if @zones.empty?
        room_ary = @zones.values.map(&:rooms).flat_map(&:values)
        return nil if room_ary.empty?
        room_ary.find { |r| r.rid == rid_or_uuid }
      # It's a UUID
      elsif /\h{8}-?\h{4}-?\h{4}-?\h{4}-?\h{12}/i.match?(rid_or_uuid)
        (room = find rid_or_uuid) ? room : find(THE_VOID_ID)
      else
        find THE_VOID_ID
      end
    end

    ###########################################################################
    def find_zone(rid_or_uuid = ZONE_ORIGIN_ID)
      if rid_or_uuid.is_a? Numeric
        @zones.values.find { |z| z.rid == rid_or_uuid }
      else rid_or_uuid =~ /\h{8}-?\h{4}-?\h{4}-?\h{4}-?\h{12}/i
        (zone = find rid_or_uuid) ? zone : find(ZONE_ORIGIN_ID)
      end
    end

    ###########################################################################
    def find_avatar(name)
      return nil if @avatars.empty?
      @avatars.values.find { |a| a.name.casecmp(name).zero? }
    end

    ###########################################################################
    def schedule(job)
      @schedule.add job
    end

    ###########################################################################
    def job_times
      @schedule.execution_times
    end

    ###########################################################################
    def schedule_stats(n)
      {
        length: @schedule.length,
        batch_size: @schedule.jobs_per_batch,
        up_next: @schedule.take(n),
      }
    end

    ###########################################################################
    def handle_jobs
      @schedule.fire_batch
    end

    ###########################################################################
    def next_room_rid
      return 1 if @zones.empty?
      rooms = @zones.values.map(&:rooms).flat_map(&:values)
      return 1 if rooms.empty?
      rooms.max_by(&:rid).rid + 1
    end

    ###########################################################################
    def next_zone_rid
      return 1 if @zones.empty?
      zone_rids = @zones.values.map &:rid
      return 1 if zone_rids.all? &:nil?
      zone_rids.max + 1
    end
  end
end
