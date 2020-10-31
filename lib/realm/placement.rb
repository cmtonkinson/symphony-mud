module Realm
  class Placement < Objekt
    attr_accessor :room, :klass, :objekt_id, :number, :max

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      @name = ''
    end

    ###########################################################################
    after_initialize lambda {
      @rid = room.next_placement_rid if room
    }

    ###########################################################################
    on_hashify lambda {
      {
        klass: klass,
        objekt_id: objekt_id,
        number: number,
        max: max
      }
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      @klass     = hash[:klass]
      @objekt_id = hash[:objekt_id]
      @number    = hash[:number]
      @max       = hash[:max]
    }

    ###########################################################################
    def target
      @target ||= $world.find(objekt_id)
    end

    ###########################################################################
    def summary
      $templates.placement_summary % [
        rid,
        klass.split('::').last,
        target.rid,
        target.name,
        number,
        max
      ]
    end

    ###########################################################################
    def place
      Symphony.logger.debug 'Placing %s' % fqid_str
      raise 'Placement Room reference is nil' if room.nil?
      raise 'Could not find Placement target' if target.nil?

      zone = room.zone
      container_name = case target
                       when Item then :items
                       when Being then :beings
                       else raise 'Objekt type not configured for Placement'
                       end

      Symphony.logger.trace '  into %s' % room.fqid_str

      # Find out how many clones of this Item are already on the ground in
      # this Zone.
      how_many = zone.rooms.values
                     .map { |r| r.send(container_name).values }
                     .flatten
                     .count { |item| item.cloned_from == target.id }

      # We already have enough; bail.
      return if how_many >= max

      # Clone
      clones = (1..number).map { |_| target.clone }
      Symphony.logger.trace '  %d clones of %s' % [clones.length, target.fqid_str]

      # Stitch
      clones.each { |clone| Logistics::Integrator.stitch clone, room }
    end
  end
end
