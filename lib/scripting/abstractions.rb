module Scripting
  module Abstraction
    #########################################################################
    def self.create_abstraction(object)
      case object
      when Realm::Being
        Being.new(object)
      when Realm::Item
        Item.new(object)
      else
        Symphony.log.error "Cannot create abstraction for object of type #{object.class}."
        nil
      end
    end

    #########################################################################
    class Being
      attr_reader :underlying

      MAPPED_CLASS = Realm::Being

      # rubocop:disable Style/SymbolProc
      METHODS = [
        {
          name: 'name',
          return_type: :string,
          params: [],
          exec: ->(underlying) { underlying.name }
        },
        {
          name: 'level',
          return_type: :int,
          params: [],
          exec: ->(underlying) { underlying.level }
        },
        {
          name: 'health',
          return_type: :int,
          params: [],
          exec: ->(underlying) { underlying.health }
        },
        {
          name: 'mana',
          return_type: :int,
          params: [],
          exec: ->(underlying) { underlying.mana }
        },
        {
          name: 'stamina',
          return_type: :int,
          params: [],
          exec: ->(underlying) { underlying.stamina }
        },
        {
          name: 'heal',
          return_type: :void,
          params: [],
          exec: ->(underlying) { underlying.heal }
        },
        {
          name: 'puts',
          return_type: :void,
          params: [:string],
          exec: ->(underlying, message) { underlying.puts message }
        }
      ]
      # rubocop:enable Style/SymbolProc

      def initialize(being)
        raise ArgumentError, 'Being abstraction must be initialized with a Being instance.' unless being.is_a? Realm::Being

        # Bind this instance to the underlying.
        @underlying = being

        # Define the methods that are allowed to be invoked inside Lua.
        METHODS.each do |method|
          define_singleton_method(method[:name]) do |*args|
            method[:exec].call(@underlying, *args)
          end
        end
      end
    end

    #########################################################################
    class Item
      attr_reader :underlying

      MAPPED_CLASS = Realm::Item

      # rubocop:disable Style/SymbolProc
      METHODS = [
        {
          name: 'name',
          return_type: :string,
          params: [],
          exec: ->(underlying) { underlying.name }
        },
        {
          name: 'level',
          return_type: :int,
          params: [],
          exec: ->(underlying) { underlying.level }
        }
      ]
      # rubocop:enable Style/SymbolProc

      def initialize(item)
        raise ArgumentError, 'Item abstraction must be initialized with an Item instance.' unless item.is_a? Realm::Item

        # Bind this instance to the underlying.
        @underlying = item

        # Define the methods that are allowed to be invoked inside Lua.
        METHODS.each do |method|
          define_singleton_method(method[:name]) do |*args|
            method[:exec].call(underlying, *args)
          end
        end
      end
    end
  end
end
