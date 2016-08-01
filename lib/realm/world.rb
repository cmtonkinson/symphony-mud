module Realm

  class World < Objekt

    ZONE_ZERO_ID = "00000000-0000-4000-a000-000000000000"
    THE_VOID_ID  = "00000000-0000-4000-a000-000000000001"

    CONTAINERS = %w(zones)

    CONTAINERS.each { |cont| attr_accessor cont }

    ###########################################################################
    def initialize(id = nil)
      super
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    end

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

  end

end
