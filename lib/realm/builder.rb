module Realm
  class Builder

    ###########################################################################
    # Generate a new Room within the given `zone`.
    def self.create_room(zone)
      new_room = Room.new nil, rid: $world.next_room_rid
      new_room.zone = zone
      zone.insert new_room
      new_room
    end

    ###########################################################################
    def self.link_existing_rooms(a, b, direction)
      # Create a new Exit
      exit = Exit.new
      exit.send "#{direction}=", b
      exit.send "#{Exit::DIRECTIONS[direction][:opposite]}=", a

      # Point the Rooms on either side to the new Exit.
      a.send "#{direction}=", exit
      b.send "#{Exit::DIRECTIONS[direction][:opposite]}=", exit

      # Add the Exit to the appropriate container. If the Rooms are in the same
      # Zone, that Zone "owns" the Exit. If the Rooms are in differing Zones,
      # then the World singleton "owns" the Exit.
      (a.zone == b.zone ? a.zone : $world).insert exit
    end

    ###########################################################################
    # Generate a new Room, within the Zone of the `existing_room`, and create
    # a bidirectional exit between the two rooms, where the exit from the
    # `existing_room` to the new Room is in `direction` and the exit from the
    # new Room to the `existing_room` is in the opposite `direction`.
    #
    # This method will also automatically set the coordinates of the new Room
    # relative to the `existing_room` according to the given `direction`.
    def self.create_linked_room(existing_room, direction)
      new_room = create_room existing_room.zone
      link_existing_rooms existing_room, new_room, direction
      new_room.coords = existing_room.coords + Exit::DIRECTIONS[direction][:transform]
      new_room
    end

    ###########################################################################
    def self.create_item(zone)
      new_item = Item.new nil, rid: zone.next_objekt_rid(Realm::Item)
      zone.insert new_item
      new_item
    end

    ###########################################################################
    def self.create_npc(zone)
      new_npc = NPC.new nil, rid: zone.next_objekt_rid(Realm::NPC)
      zone.insert new_npc
      new_npc 
    end

  end
end
