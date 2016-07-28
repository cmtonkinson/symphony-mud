module Realm

  class Zone < Objekt
    attr_accessor :rooms, :items, :npcs

    ###########################################################################
    def initialize(id = nil)
      super id
      @rooms = Util::AttrHash.new
      @items = Util::AttrHash.new
      @npcs  = Util::AttrHash.new
    end

  end

end
