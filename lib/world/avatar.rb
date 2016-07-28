module World

  class Avatar < Objekt

    attr_accessor :room

    ###########################################################################
    def initialize(id = nil, name = nil)
      super id
      @name = name
    end

    ###########################################################################

  end

end
