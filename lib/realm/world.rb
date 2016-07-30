module Realm

  class World < Objekt

    CONTAINERS = %w(zones)

    attr_reader :room_zero
    CONTAINERS.each { |cont| attr_accessor cont }

    ###########################################################################
    def initialize(id = nil)
      super id
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
      load_zones
    end

    ###########################################################################
    def load_zones
      Realm::Scribe.the_story_of_baron_munchausen.each do |zone_id|
        @zones << Realm::Scribe.load(Realm::Zone, zone_id)
      end
    end

  end

end
