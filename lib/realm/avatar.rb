module Realm

  class Avatar < Being

    attr_accessor :room

    ###########################################################################
    def initialize(id = nil, name = nil)
      super id
      @name = name
      @room = $world.find_room Realm::World::THE_VOID_ID
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

  end

end
