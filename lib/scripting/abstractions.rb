module Scripting
  module Abstraction
    #########################################################################
    class Being
      MAPPED_CLASS = Realm::Being
      METHODS = [
        {
          name: 'get_name',
          return_type: :string,
          params: [],
          exec: ->(being) { being.name }
        },
        {
          name: 'get_level',
          return_type: :int,
          params: [],
          exec: ->(being) { being.level }
        },
        {
          name: 'get_health',
          return_type: :int,
          params: [],
          exec: ->(being) { being.health }
        },
        {
          name: 'get_mana',
          return_type: :int,
          params: [],
          exec: ->(being) { being.mana }
        },
        {
          name: 'get_stamina',
          return_type: :int,
          params: [],
          exec: ->(being) { being.stamina }
        },
        {
          name: 'heal',
          return_type: :void,
          params: [],
          exec: ->(being) { being.heal }
        },
        {
          name: 'puts',
          return_type: :void,
          params: [:string],
          exec: ->(being, message) { being.puts message }
        }
      ]

      def initialize(being)
        unless being.is_a? Realm::Being
          raise ArgumentError, 'Being abstraction must be initialized with a Being instance.'
        end

        # Bind this instance to the being.
        @being = being

        # Define the methods that are allowed to be invoked inside Lua.
        METHODS.each do |method|
          define_singleton_method(method[:name]) do |*args|
            method[:exec].call(@being, *args)
          end
        end
      end
    end

    #########################################################################
    class Item
      MAPPED_CLASS = Realm::Item
      METHODS = [
        {
          name: 'get_name',
          return_type: :string,
          params: [],
          exec: ->(item) { item.name }
        },
        {
          name: 'get_level',
          return_type: :int,
          params: [],
          exec: ->(item) { item.level }
        }
      ]

      def initialize(item)
        unless being.is_a? Realm::Item
          raise ArgumentError, 'Item abstraction must be initialized with an Item instance.'
        end

        # Bind this instance to the item.
        @item = item

        # Define the methods that are allowed to be invoked inside Lua.
        METHODS.each do |method|
          define_singleton_method(method[:name]) do |*args|
            method[:exec].call(item, *args)
          end
        end
      end
    end
  end
end
