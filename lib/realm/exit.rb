require "matrix"

module Realm
  class Exit < Objekt

    DIRECTIONS = {
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

    DIRECTIONS.keys.each { |exit| attr_accessor exit }

    simple_flag :open, true, :open?, :open!, :closed?, :close!
    simple_flag :lock, false, :locked?, :lock!, :unlocked?, :unlock!

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      DIRECTIONS.keys.each { |exit| send "#{exit}=", nil }
    end

    ###########################################################################
    on_hashify -> {
      # We want a Hash mapping each direction to the id of the Room to which
      # it points, or nil.
      exit_room_ids = Hash[DIRECTIONS.keys.map { |dir|
        target_room = send dir
        [dir, target_room ? target_room.id : nil]
      }]

      {
        default_open: default_open,
        default_lock: default_lock,
      }.merge! exit_room_ids
    }

    ###########################################################################
    on_dehashify -> hash {
      @open = @default_open = hash[:default_open]
      @lock = @default_lock = hash[:default_lock]

      DIRECTIONS.keys.each { |dir|
        send "#{dir}=", $world.find(hash[dir])
      }
    }

  end
end
