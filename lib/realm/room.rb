module Realm
  class Room < Objekt
    CONTAINERS = %w[beings items placements].freeze

    attr_accessor :zone, :coords

    CONTAINERS.each { |cont| attr_accessor cont }
    Realm::Exit::DIRECTIONS.each_key { |exit| attr_accessor exit }

    def initialize(id = nil, props = {})
      super
    end

    after_initialize lambda {
      @zone   = nil
      @coords = Vector[0, 0, 0]
      Realm::Exit::DIRECTIONS.each_key { |exit| send "#{exit}=", nil }
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    on_hashify lambda {
      {
        zone: @zone.id,
        coords: @coords.to_a.join(','),
        placements: @placements.values.map(&:hashify)
      }
    }

    on_dehashify lambda { |hash|
      @zone   = $world.find hash[:zone]
      @coords = Vector[*hash[:coords].split(',').map(&:to_i)]
      if hash.key? :placements
        hash[:placements].each do |ser|
          p = Realm::Placement.dehashify ser
          p.room = self
          insert p
        end
      end
    }

    Realm::Exit::DIRECTIONS.each_key do |exit|
      define_method "#{exit}_room", (proc do
        # Yo dawg, I heard you like to send exit to your send exit.
        send(exit).send(exit)
      end)
    end

    %w[x y z].each_with_index do |coord_name, coord_index|
      define_method "#{coord_name}_coordinate" do
        @coords[coord_index]
      end
      alias_method :"#{coord_name}", :"#{coord_name}_coordinate"
    end

    def puts(format, *args, **opts)
      dont_message = if opts.key?(:exclude)
                       opts[:exclude].is_a?(Array) ? opts[:exclude] : [opts[:exclude]]
                     else
                       []
                     end

      send_to = @beings.values - dont_message
      send_to.each do |being|
        being.puts Util::Linguist.interpret being, format, *args
      end
    end

    def indented_puts(spaces, format, *, **)
      indented_format = (' ' * spaces) + format
      puts(indented_format, *, **)
    end

    def find_being(name)
      beings.values.find { |b| b.name.index(name)&.zero? }
    end

    def next_placement_rid
      rids = placements.values.map(&:rid)
      return 1 if rids.empty?

      rids.max + 1
    end

    def summary
      $templates.room_summary % [
        rid, name,
        id,
        zone.name
      ]
    end

    def reset
      @placements.values.each(&:place)
    end
  end
end
