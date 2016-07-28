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
      @room = nil # again, global lookup
    end

  end

end
