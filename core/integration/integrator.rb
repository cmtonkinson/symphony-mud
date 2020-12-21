module Logistics
  class Integrator
    ###########################################################################
    # Stitching is the process of placing a newly created Objekt into the game.
    #
    # Used, for example, when spawing NPCs and Items, to take the freshly
    # minted Objekt, add it to the appropriate container, run any hooks needed,
    # and set any attributes that are required to fully integrate the Objekt
    # into the game world. This helps abstract the object and memory management
    # portion of Objekt creation from the game-mechanics part of Objekt
    # creation.
    def self.stitch(objekt, target_container)
      # Add the Objekt to the container.
      objekt.add.to target_container

      case objekt
      when Realm::Being then being_stitch objekt, target_container
      when Realm::Room then room_stitch objekt, target_container
      end

      # Animate (as appropriate).
      objekt.run_hook :vivify
    end

    ###########################################################################
    def self.being_stitch(being, container)
      unless container.is_a? Realm::Room
        raise 'Can only stitch a Being into a Room (being = %s, container = %s)' % [being.fqid_str, container.fqid_str]
      end

      being.room = container
    end

    ###########################################################################
    def self.room_stitch(room, container)
      unless container.is_a? Realm::Zone
        raise 'Can only stitch a Room into a Zone (room = %s, container = %s)' % [room.fqid_str, container.fqid_str]
      end

      room.zone = container
    end

    ###########################################################################
    # This is used to provide an interface between the Effect and the Objekt
    # it is being applied to that is a clean abstraction for the client code.
    def self.apply_effect(type, victim, actor = nil)
      type_sym = type.capitalize
      effect = Realm::Effects.const_get(type_sym).new(nil, {
                                                        target: victim,
                                                        originator: actor
                                                      })
      effect.apply
    end
  end
end
