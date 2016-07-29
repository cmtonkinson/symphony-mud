require "matrix"

module Realm

  class Room < Objekt

    CONTAINERS = %w(avatars)

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

    CONTAINERS.each { |cont| attr_accessor cont }
    EXITS.keys.each { |exit| attr_accessor exit }

    ###########################################################################
    def initialize(id = nil)
      super id
      @zone   = nil
      @coords = Vector[0, 0, 0]
      EXITS.keys.each { |exit| send "#{exit}=", nil }
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    end

    ###########################################################################
    def hashify
      {
        zone: @zone.id,
        coords: @coords.to_a.join(","),
      }
    end

    ###########################################################################
    def dehashify(hash)
      @zone   = Util::Registry.lookup hash[:zone]
      @coords = Vector[*hash[:coords].split(",")]
    end

  end

end
