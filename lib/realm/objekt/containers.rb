module Realm
  class Objekt
    attr_reader :containers

    after_initialize lambda {
      # By adding `Util::AttrHash` instance members to your subclasses, you
      # can easily keep collections of Objekts organized. If you decide to
      # use the Objekt methods #insert and #delete as abstractions over these
      # containers, Objekt will automatically keep track of which containers
      # your Objekt is held in. In other words, the #insert and #delete helpers
      # will bookkeep the bidirectional association between a container and
      # each Objekt in it.
      @containers = Set.new
    }

    enervate lambda {
      # When destroying an Objekt, we want to be sure it's removed from all
      # containers it may be in (which should theoretically only be one at a
      # time, but that may not always be true - hence @containers being an
      # Array) or we could wind up with strange leaks.
      @containers.each { |container| container.delete self }
    }

    # Given an obj of type Thing
    # Attempts to call `@things.insert obj`
    def insert(objekt)
      container = container_for objekt
      container.insert objekt
      objekt.containers.add self
    end

    # Given an obj of type Thing
    # Attempts to call `@things.remove obj`
    def delete(objekt)
      container = container_for objekt
      container.delete objekt
      objekt.containers.delete self
    end

    def container_for(obj)
      raise ArgumentError, "#{obj.class.name} is not a subclass of Objekt" unless obj.is_a? Objekt

      container_name = nil

      obj.class.ancestors.each do |klass|
        # If we've reached up the object heirarchy all the way to Objekt, then
        # there are no valid containers configured for this obj.class or any of
        # its ancestors.
        raise ArgumentError, "No #{obj.class.name} container configured on #{self.class.name}" if klass == Objekt

        # Construct a container name from the klass.
        container_name = klass.name.split('::').last.downcase.to_s + 's'

        # If the Objekct responds to the container name, then we've found the
        # most-specific container configured to accept objects of this type.
        break if respond_to? container_name
      end

      send container_name
    end
  end
end
