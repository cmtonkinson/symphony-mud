module World

  class Zone < Objekt
    attr_accessor :rooms, :items, :npcs

    ###########################################################################
    def initialize
      @rooms = Util::AttrHash.new
      @items = Util::AttrHash.new
      @npcs  = Util::AttrHash.new
    end

  end

end
