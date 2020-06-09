require 'matrix'

module Realm
  class Exit < Objekt
    DIRECTIONS = {
      'north' => {
        opposite: 'south',
        transform: Vector[0, 1, 0]
      },
      'east' => {
        opposite: 'west',
        transform: Vector[1, 0, 0]
      },
      'south' => {
        opposite: 'north',
        transform: Vector[0, -1, 0]
      },
      'west' => {
        opposite: 'east',
        transform: Vector[-1, 0, 0]
      },
      'up' => {
        opposite: 'down',
        transform: Vector[0, 0, 1]
      },
      'down' => {
        opposite: 'up',
        transform: Vector[0, 0, -1]
      }
    }

    DIRECTIONS.each_key { |exit| attr_accessor exit }

    simple_flag :open, true, :open?, :open!, :closed?, :close!
    simple_flag :lock, false, :locked?, :lock!, :unlocked?, :unlock!

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      DIRECTIONS.each_key { |exit| send "#{exit}=", nil }
    end

    ###########################################################################
    on_hashify lambda {
      # We want a Hash mapping each direction to the id of the Room to which
      # it points, or nil.
      exit_room_ids = Hash[DIRECTIONS.keys.map do |dir|
        target_room = send dir
        [dir, target_room ? target_room.id : nil]
      end]

      {
        default_open: default_open,
        default_lock: default_lock
      }.merge! exit_room_ids
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      @open = @default_open = hash[:default_open]
      @lock = @default_lock = hash[:default_lock]

      DIRECTIONS.each_key do |dir|
        send "#{dir}=", $world.find(hash[dir])
      end
    }

    ###########################################################################
    # If the given string matches the prefix of a defined direction, return
    # the full string name of that direction. Otherwise, return nil.
    def self.direction?(str)
      test = DIRECTIONS.keys.find { |name| name.index(str) == 0 }
      test.nil? ? nil : test
    end
  end
end
