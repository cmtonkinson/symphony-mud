module Realm

  class Being < Objekt

    attr_accessor :room

    ###########################################################################
    def initialize(id = nil)
      super
      @output_buffer = ""
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
    %i(print puts).each do |meth|
      define_method meth do |message|
        @output_buffer << message
      end
    end

    ###########################################################################
    # NOTE Command::Operator has a dependency on the interface of Base::User.
    # This method will work as long as that dependency is limited to a single
    # property called `avatar`, since we're basically passing a Stub object
    # in the form of an OpenStruct.
    def command_proxy(command_string)
      @proxy_operator ||= Command::Operator.new OpenStruct.new(avatar: self)
      @proxy_operator.handle command_string
    end
    alias_method :cmd, :command_proxy

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
      cmd "look"
    end

  end

end
