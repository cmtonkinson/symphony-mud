module World

  class Builder

    ###########################################################################
    # Generate a new Room within the given `zone`.
    def self.create_room(zone)
      new_room = Room.new
      new_room.zone = zone
      zone << new_room
      new_room
    end

    ###########################################################################
    def self.link_existing_rooms(a, b, direction)
      a.send "#{direction}=", b
      b.send "#{Room::EXITS[direction][:opposite]}=", a
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
      new_room.coords = existing_room.coords + Room::EXITS[direction][:transform]
      new_room
    end

  end

end
