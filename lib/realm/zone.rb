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

      hash[:exits].each do |hash|
        exit = Realm::Exit.dehashify hash

        # Add the Exit to the Zone's collection.
        @exits << exit

        Realm::Exit::DIRECTIONS.keys.each do |dir|
          # Most links will be nil.
          room = exit.send dir
          next if room.nil?
          # Point the Room to this Exit (the link directions are opposing).
          room.send "#{Realm::Exit::DIRECTIONS[dir][:opposite]}=", exit
        end

      end
    end

  end

end
