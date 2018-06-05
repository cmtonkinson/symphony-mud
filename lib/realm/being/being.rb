module Realm
  class Being < Objekt

    CONTAINERS = %w(inventory)

    attr_accessor :room, :gender
    attr_accessor :abilities, :enablers, :klass

    CONTAINERS.each { |cont| attr_accessor cont }

    define_hooks :before_room_change, :after_room_change

    ###########################################################################
    def initialize(id = nil, props = {})
      @gender        = Gender::NEUTER
      @output_buffer = ''
      @abilities     = Array.new
      @enablers      = Array.new
      @klass         = nil
      super
    end

    ###########################################################################
    after_initialize -> {
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    on_hashify -> {
      hash = {
        klass: @klass.name,
        room: @room&.id,
        gender: @gender,
        inventory: @inventory.values.map(&:hashify),
      }

      { being: hash }
    }

    ###########################################################################
    on_dehashify -> hash {
      hash = hash[:being]

      if hash[:klass]
        @klass = Talent.const_get(hash[:klass].capitalize).new
        add_enabler @klass
      end

      unless is_a? NPC
        @room = $world.find_room hash[:room]
        @room.insert self
      end

      @gender = hash[:gender]

      hash[:inventory].each { |hash| @inventory.insert Realm::Item.dehashify(hash) }
    }

    ###########################################################################
    vivify -> {
      $world.beings.insert self
    }
    
    ###########################################################################
    enervate -> {
      $world.beings.delete self
      @room.delete self
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
      @output_buffer = ''
      buf
    end

    ###########################################################################
    def prompt
      "\n%s[{G%d{x/{g%d{xhp {C%d{x/{c%d{xmana {M%d{xstamina {y%d{xtnl] " % [
        prompt_prefix,
        health.current, health.max,
        mana.current, mana.max,
        stamina.current,
        tnl,
      ]
    end

    ###########################################################################
    def prompt_prefix
      _prompt_prefix
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
    def can_go?(direction)
      exit = @room.send direction
      return I18n.t('movement.no_exit') if exit.nil?
      return I18n.t('movement.closed') if exit.closed?
      true
    end

    ###########################################################################
    def go(direction)
      src  = @room
      exit = @room.send direction
      dst  = exit.send direction

      # Notify.
      puts I18n.t('being.you_go'), direction
      src.puts I18n.t('being.leaves'), self, direction, exclude: self
      dst.puts I18n.t('being.arrived'), self, direction

      # Move.
      transport dst
    end

    ###########################################################################
    def transport(dst)
      src = @room

      self.run_hook :before_room_change, src, dst
      move.from(src.beings).to(dst.beings)
      @room = dst
      self.run_hook :after_room_change, src, dst 

      # Look around.
      cmd 'look'
    end

    ###########################################################################
    def other_beings_in_room
      (@room.beings.values.reject { |b| b == self }) || []
    end
    alias_method :other_beings, :other_beings_in_room

    ###########################################################################
    def add_enabler(enabler)
      @enablers.push enabler
      enabler.abilities.each { |a| add_ability a }
    end

    ###########################################################################
    def add_ability(ability)
      @abilities.push ability
      ability.added_to self
    end

    ###########################################################################
    def get_ability(name)
      @abilities.first { |a| a.name == name }
    end

  end
end

