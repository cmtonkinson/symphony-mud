module Realm

  class Room < Objekt

    CONTAINERS = %w(beings)

    attr_accessor :zone, :items, :npcs
    attr_accessor :coords

    CONTAINERS.each { |cont| attr_accessor cont }
    Realm::Exit::DIRECTIONS.keys.each { |exit| attr_accessor exit }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      @zone   = nil
      @coords = Vector[0, 0, 0]
      Realm::Exit::DIRECTIONS.keys.each { |exit| send "#{exit}=", nil }
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    end

    ###########################################################################
    def _hashify
      {
        zone: @zone.id,
        coords: @coords.to_a.join(","),
      }
    end

    ###########################################################################
    def _dehashify(hash)
      @zone   = $world.find hash[:zone]
      @coords = Vector[*hash[:coords].split(",")]
    end

    ###########################################################################
    Realm::Exit::DIRECTIONS.keys.each do |exit|
      define_method "#{exit}_room", Proc.new {
        # Yo dawg, I heard you like to send exit to your send exit.
        send(exit).send(exit)
      }
    end

    ###########################################################################
    def puts(format, *args)
      message = Util::Linguist.interpret format, *args
      @beings.values.each do |being|
        being.puts message
      end
    end

  end

end
