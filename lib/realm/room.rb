module Realm
  class Room < Objekt

    CONTAINERS = %w(beings inventory)

    attr_accessor :zone, :items, :npcs
    attr_accessor :coords

    CONTAINERS.each { |cont| attr_accessor cont }
    Realm::Exit::DIRECTIONS.keys.each { |exit| attr_accessor exit }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
      @zone   = nil
      @coords = Vector[0, 0, 0]
      Realm::Exit::DIRECTIONS.keys.each { |exit| send "#{exit}=", nil }
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    on_hashify -> {
      {
        zone: @zone.id,
        coords: @coords.to_a.join(","),
      }
    }

    ###########################################################################
    on_dehashify -> hash {
      @zone   = $world.find hash[:zone]
      @coords = Vector[*hash[:coords].split(",").map(&:to_i)]
    }

    ###########################################################################
    Realm::Exit::DIRECTIONS.keys.each do |exit|
      define_method "#{exit}_room", Proc.new {
        # Yo dawg, I heard you like to send exit to your send exit.
        send(exit).send(exit)
      }
    end

    ###########################################################################
    %w(x y z).each_with_index do |coord_name, coord_index|
      define_method "#{coord_name}_coordinate" do
        @coords[coord_index]
      end
      alias_method :"#{coord_name}", :"#{coord_name}_coordinate"
    end

    ###########################################################################
    def puts(format, *args, **opts)
      message = Util::Linguist.interpret format, *args
      @beings.values.each do |being|
        if opts.key?(:exclude)
          dont_message = opts[:exclude].is_a?(Array) ? opts[:exclude] : [opts[:exclude]]
          next if dont_message.include? being
        end
        being.puts message
      end
    end

    ###########################################################################
    def find_being(name)
      beings.values.find { |b| b.name.index(name) == 0 }
    end

    ###########################################################################
    def summary
      $templates.room_summary % [
        rid, name,
        id,
        zone.name,
      ]
    end

  end
end
