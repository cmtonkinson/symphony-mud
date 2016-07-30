module Realm

  class Avatar < Objekt

    attr_accessor :room

    ###########################################################################
    def initialize(id = nil, name = nil)
      super id
      @name = name
    end

    ###########################################################################
    def hashify
      {
        room: @room.id
      }
    end

    ###########################################################################
    def dehashify(hash)
      @room = $world.find_room hash[:room]
    end

  end

end
