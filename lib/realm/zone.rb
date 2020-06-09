module Realm
  class Zone < Objekt
    include Realm

    CONTAINERS = %w[rooms exits items npcs].freeze

    CONTAINERS.each { |c| attr_accessor c.to_sym }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize lambda {
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    on_hashify lambda {
      {
        rooms: @rooms.values.map(&:hashify),
        exits: @exits.values.map(&:hashify),
        items: @items.values.map(&:hashify),
        npcs: @npcs.values.map(&:hashify)
      }
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      hash[:rooms].each { |h| @rooms.insert Realm::Room.dehashify(h) }

      hash[:exits].each do |h|
        exit = Realm::Exit.dehashify h

        # Add the Exit to the Zone's collection.
        @exits.insert exit

        Realm::Exit::DIRECTIONS.each_key do |dir|
          # Most links will be nil.
          room = exit.send dir
          next if room.nil?

          # Point the Room to this Exit (the link directions are opposing).
          room.send "#{Realm::Exit::DIRECTIONS[dir][:opposite]}=", exit
        end
      end

      hash[:items].each { |h| @items.insert Realm::Item.dehashify(h) }
      hash[:npcs].each { |h| @npcs.insert Realm::NPC.dehashify(h) }
    }

    ###########################################################################
    vivify lambda {
      reset
      $world.schedule Mechanics::RecurringJob.new 90, self, 90, :reset
    }

    ###########################################################################
    def layout
      # Create a 2D Array that is large enough to hold the shape of the Zone.
      x_min, x_max  = extreme_coords 0
      width         = x_max - x_min + 1
      x_translation = x_min.negative? ? -x_min : 0

      y_min, y_max  = extreme_coords 1
      height        = y_max - y_min + 1
      y_translation = y_min.negative? ? -y_min : 0

      layout = Array.new(height * 2) { Array.new(width * 2) { nil } }

      # Set a value for each coordinate that is occupied by a Room.
      @rooms.each_value do |room|
        # Get the X,Y for the Room and set it in the layout.
        adjusted_x = (room.x + x_translation) * 2
        adjusted_y = (room.y + y_translation) * 2
        layout[adjusted_y][adjusted_x] = room
        # Get the X,Y for each Exit.
        # NOTE We can take a shortcut here, knowing that Exits are always
        # bidirectional means that we only have to examine the North and East
        # direction of each Exit and we'll have accounted for all intrazone
        # links.
        %i[north east].each do |direction|
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
      get_coord = proc { |r| r.coords[index] }
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
        @rooms.length, @items.length, @npcs.length
      ]
    end

    ###########################################################################
    def reset
      Symphony.logger.debug 'Zone reset: %s' % fqid_str
      @rooms.values.each(&:reset)
    end
  end
end
