require "securerandom"

module Zone

  class Zone
    attr_reader :id
    attr_accessor :title, :rooms, :items, :npcs

    def initialize
      @id    = SecureRandom.uuid
      @title = "_title"
      @rooms = Util::AttrHash.new
      @items = Util::AttrHash.new
      @npcs  = Util::AttrHash.new
    end

  end

end
