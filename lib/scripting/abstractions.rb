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
      MAPPED_CLASS = Realm::Being
      METHODS = [
        {
          name: 'name',
          return_type: :string,
          params: [],
          exec: lambda(&:name)
        },
        {
          name: 'level',
          return_type: :int,
          params: [],
          exec: lambda(&:level)
        },
        {
          name: 'health',
          return_type: :int,
          params: [],
          exec: lambda(&:health)
        },
        {
          name: 'mana',
          return_type: :int,
          params: [],
          exec: lambda(&:mana)
        },
        {
          name: 'stamina',
          return_type: :int,
          params: [],
          exec: lambda(&:stamina)
        },
        {
          name: 'heal',
          return_type: :void,
          params: [],
          exec: lambda(&:heal)
        },
        {
          name: 'puts',
          return_type: :void,
          params: [:string],
          exec: ->(being, message) { being.puts message }
        }
      ]

      def initialize(being)
        raise ArgumentError, 'Being abstraction must be initialized with a Being instance.' unless being.is_a? Realm::Being

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
          name: 'name',
          return_type: :string,
          params: [],
          exec: lambda(&:name)
        },
        {
          name: 'level',
          return_type: :int,
          params: [],
          exec: lambda(&:level)
        }
      ]

      def initialize(item)
        raise ArgumentError, 'Item abstraction must be initialized with an Item instance.' unless being.is_a? Realm::Item

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
