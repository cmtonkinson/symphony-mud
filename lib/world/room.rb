require "matrix"

module World

  class Room < Objekt

    EXITS = {
      "north" => {
        opposite: "south",
        transform: Vector[0, 1, 0],
      },
      "east" => {
        opposite: "west",
        transform: Vector[1, 0, 0],
      },
      "south" => {
        opposite: "north",
        transform: Vector[0, -1, 0],
      },
      "west" => {
        opposite: "east",
        transform: Vector[-1, 0, 0],
      },
      "up" => {
        opposite: "down",
        transform: Vector[0, 0, 1],
      },
      "down" => {
        opposite: "up",
        transform: Vector[0, 0, -1],
      },
    }

    attr_accessor :zone, :items, :npcs
    attr_accessor :coords
    EXITS::keys.each { |exit| attr_accessor exit }

    ###########################################################################
    def initialize(id = nil)
      super id
      @zone   = nil
      @items  = Util::AttrHash.new
      @npcs   = Util::AttrHash.new
      @coords = Vector[0, 0, 0]
      EXITS::keys.each { |exit| send "#{exit}=", nil }
    end

  end

end
