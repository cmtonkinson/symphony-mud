module Realm

  class Being < Objekt

    attr_accessor :room

    ###########################################################################
    def initialize(id = nil)
      super
    end

    ###########################################################################
    def _hashify
      {
        room: @room.id
      }
    end

    ###########################################################################
    def _dehashify(hash)
      @room = $world.find_room hash[:room]
    end

    ###########################################################################
    # Returns true or a String containing an error message.
    def can_move?(direction)
      exit = @room.send direction
      return I18n.t("movement.no_exit") if exit.nil?
      return I18n.t("movement.closed") if exit.closed?
      true
    end

    ###########################################################################
    def move(direction)
      from = @room
      exit = @room.send direction
      to   = exit.send direction

      # Perform the actual switch.
      from - self
      to << self
      @room = to

      # Look around.
      operator = Command::Operator.new self.user, Command.configuration.command_sets[:global]
      operator.handle "look"
    end

  end

end
