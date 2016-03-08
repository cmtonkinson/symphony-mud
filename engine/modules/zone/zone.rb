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

    # Given an obj of type Thing
    # Attempts to send `@things << obj`
    def insert(obj)
      container = obj.class.name.downcase.to_sym + "s"
      raise ArgumentError.new "Cannot insert object of type #{obj.class.name}" unless respond_to? container
      send(container) << obj
    end
    alias_method :<<, :insert

  end

end
