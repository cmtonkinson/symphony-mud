module Realm
  class Being < Objekt

    attr_accessor :room, :gender

    ###########################################################################
    def initialize(id = nil, props = {})
      @gender        = Gender::NEUTER
      @output_buffer = ""
      super
    end

    ###########################################################################
    on_hashify -> {
      hash = {
        room: @room.id,
        gender: @gender,
      }

      {being: hash}
    }

    ###########################################################################
    on_dehashify -> hash {
      hash = hash[:being]
      @room = $world.find_room hash[:room]
      @room.insert self

      @gender = hash[:gender]
    }

    ###########################################################################
    def print(format, *args)
      @output_buffer << Util::Linguist.interpret(format, *args)
    end

    ###########################################################################
    def puts(format, *args)
      print format, *args
      @output_buffer << "\n" unless @output_buffer[-1] == "\n"
    end

    ###########################################################################
    def output?
      !@output_buffer.empty?
    end

    ###########################################################################
    def output!
      buf = @output_buffer.dup + prompt
      @output_buffer = ""
      buf
    end

    ###########################################################################
    def prompt
      "\n[{G%d{x/{g%d{xhp {C%d{x/{c%d{xmana {M%d{xstamina {y%d{xtnl] " % [
        health.current, health.max,
        mana.current, mana.max,
        stamina.current,
        tnl,
      ]
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

      # Notify.
      puts I18n.t("being.you_go"), direction
      from.puts I18n.t("being.leaves"), self, direction, exclude: self
      to.puts I18n.t("being.arrived"), self, direction

      # Move.
      transport to
    end

    ###########################################################################
    def transport(to)
      # Swap.
      @room.remove self
      @room = to
      @room.insert self

      # Look around.
      cmd "look"
    end

    ###########################################################################
    def other_beings_in_room
      (@room.beings.values.reject { |b| b == self }) || []
    end
    alias_method :other_beings, :other_beings_in_room

  end
end
