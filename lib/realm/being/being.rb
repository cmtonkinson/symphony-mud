module Realm
  class Being < Objekt
    CONTAINERS = %w(items)

    attr_accessor :room, :gender, :hand
    attr_accessor :enablers, :klass, :anatomy

    CONTAINERS.each { |cont| attr_accessor cont }

    define_hooks :before_room_change, :after_room_change

    ###########################################################################
    def initialize(id = nil, props = {})
      @output_buffer = ''
      @gender        = Gender::NEUTER
      @hand          = nil
      @enablers      = Array.new
      @klass         = nil
      @anatomy       = nil
      @hand          = 'right'
      super
    end

    ###########################################################################
    after_initialize -> {
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    }

    ###########################################################################
    on_hashify -> {
      hash = {
        klass: @klass&.name,
        anatomy: @anatomy&.name,
        room: @room&.id,
        gender: @gender,
        hand: @hand,
        items: @items.values.map(&:hashify),
      }

      { being: hash }
    }

    ###########################################################################
    on_dehashify ->hash {
      hash = hash[:being]

      ::Talent.const_get(hash[:klass].capitalize).new(self) if hash[:klass]
      ::Talent.const_get(hash[:anatomy].capitalize).new(self) if hash[:anatomy]

      unless is_a? NPC
        @room = $world.find_room hash[:room]
        @room.insert self
      end

      @gender = hash[:gender]
      @hand = hash[:hand]

      hash[:items].each { |h| insert Realm::Item.dehashify(h) }
    }

    ###########################################################################
    on_deep_copy -> (original) {
      ::Talent.const_get(original.anatomy.name.capitalize).new(self) if original.anatomy
      ::Talent.const_get(original.klass.name.capitalize).new(self) if original.klass
    }

    ###########################################################################
    vivify -> {
      $world.beings.insert self
    }

    ###########################################################################
    enervate -> {
      $world.beings.delete self
      @room.delete self

      items.each { |i| i.run_hook :enervate }
      anatomy.slots do |slot|
        i = slot.set nil
        i.run_hook :enervate if i
      end if anatomy

      dispose
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
    def indented_puts(spaces, format, *args)
      indented_format = (' ' * spaces) + format
      puts indented_format, *args
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

  end
end
