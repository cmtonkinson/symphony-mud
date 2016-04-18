module World

  class Room < Objekt
    attr_accessor :items, :npcs

    ###########################################################################
    def initialize
      @items = Util::AttrHash.new
      @npcs  = Util::AttrHash.new
    end

  end

end
