module Realm
  class Zone < Objekt

    include Realm

    CONTAINERS = %w(rooms exits items npcs)

    CONTAINERS.each { |c| attr_accessor c.to_sym }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    on_hashify -> {
      {
        rooms: @rooms.values.map(&:hashify),
        exits: @exits.values.map(&:hashify),
        items: @items.values.map(&:hashify),
        npcs: @npcs.values.map(&:hashify),
      }
    }

    ###########################################################################
    on_dehashify -> hash {
      hash[:rooms].each { |hash| @rooms.insert Realm::Room.dehashify(hash) }

      hash[:exits].each do |hash|
        exit = Realm::Exit.dehashify hash

        # Add the Exit to the Zone's collection.
        @exits.insert exit

        Realm::Exit::DIRECTIONS.keys.each do |dir|
          # Most links will be nil.
          room = exit.send dir
          next if room.nil?
          # Point the Room to this Exit (the link directions are opposing).
          room.send "#{Realm::Exit::DIRECTIONS[dir][:opposite]}=", exit
        end
      end

      hash[:items].each { |hash| @items.insert Realm::Item.dehashify(hash) }
      hash[:npcs].each { |hash| @npcs.insert Realm::NPC.dehashify(hash) }
    }

    ###########################################################################
    def get_layout
      # Create a 2D Array that is large enough to hold the shape of the Zone.
      x_min, x_max  = extreme_coords 0
      width         = x_max - x_min + 1
      x_translation = x_min < 0 ? -x_min : 0

      y_min, y_max  = extreme_coords 1
      height        = y_max - y_min + 1
      y_translation = y_min < 0 ? -y_min : 0

      translation = Vector[x_translation, y_translation, 0]
      layout      = Array.new(height * 2) { Array.new(width * 2) { nil } }

      # Set a value for each coordinate that is occupied by a Room.
      @rooms.values.each do |room|
        # Get the X,Y for the Room and set it in the layout.
        adjusted_x = (room.x + x_translation) * 2
        adjusted_y = (room.y + y_translation) * 2
        layout[adjusted_y][adjusted_x] = room
        # Get the X,Y for each Exit.
        # NOTE We can take a shortcut here, knowing that Exits are always
        # bidirectional means that we only have to examine the North and East
        # direction of each Exit and we'll have accounted for all intrazone
        # links.
        marker = ""
        %i(north east).each do |direction|
          exit_x = adjusted_x
          exit_y = adjusted_y
          next if room.send(direction).nil?
          case direction
          when :north then exit_y += 1
          when :east  then exit_x += 1
          end
          layout[exit_y][exit_x] = room.send(direction)
        end
      end

      # Return the structure.
      layout
    end

    ###########################################################################
    def extreme_coords(index)
      room_ary = @rooms.values
      get_coord = Proc.new { |r| r.coords[index] }
      [
        room_ary.min_by(&get_coord).coords[index],
        room_ary.max_by(&get_coord).coords[index]
      ]
    end

    ###########################################################################
    def summary
      $templates.zone_summary % [
        rid, name,
        id,
        @rooms.length, @items.length, @npcs.length,
      ]
    end

  end
end

