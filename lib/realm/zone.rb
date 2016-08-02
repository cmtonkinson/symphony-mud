module Realm

  class Zone < Objekt

    CONTAINERS = %w(rooms exits)

    CONTAINERS.each { |c| attr_accessor c.to_sym }

    ###########################################################################
    def initialize(id = nil)
      super
      CONTAINERS.each { |c| instance_variable_set "@#{c}", Util::AttrHash.new }
    end

    ###########################################################################
    def _hashify
      {
        rooms: @rooms.values.map(&:hashify),
        exits: @exits.values.map(&:hashify),
      }
    end

    ###########################################################################
    def _dehashify(hash)
      hash[:rooms].each { |hash| @rooms << Realm::Room.dehashify(hash) }
      hash[:exits].each { |hash| @exits << Realm::Exit.dehashify(hash) }
    end

  end

end
