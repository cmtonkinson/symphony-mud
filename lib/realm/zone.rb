module Realm

  class Zone < Objekt

    CONTAINERS = %w(rooms)

    CONTAINERS.each { |c| attr_accessor c.to_sym }

    ###########################################################################
    def initialize(id = nil)
      super id
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    end

    ###########################################################################
    def _hashify
      {
        rooms: @rooms.values.map(&:hashify),
      }
    end

  end

end
