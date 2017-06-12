module Realm

  class Placement < Objekt
    attr_accessor :room, :klass, :objekt_id, :number, :max

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
      @rid = room.next_placement_rid if room
    }

    ###########################################################################
    on_hashify -> {
      {
        klass: klass,
        objekt_id: objekt_id,
        number: number,
        max: max,
      }
    }

    ###########################################################################
    on_dehashify -> hash {
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
        klass.split("::").last,
        target.rid,
        target.name,
        number,
        max,
      ]
    end

  end

end

