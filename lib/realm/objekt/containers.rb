require "set"

module Realm

  class Objekt

    attr_accessor :containers

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to call `@things.insert obj`
    def insert(objekt)
      container = container_for objekt
      container.insert objekt
      objekt.containers.add self
    end

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to call `@things.remove obj`
    def remove(objekt)
      container = container_for objekt
      container.remove objekt
      objekt.containers.delete self
    end

    ###########################################################################
    def container_for(obj)
      raise ArgumentError.new "#{obj.class.name} is not a subclass of Objekt" unless obj.is_a? Objekt
      container_name = nil

      obj.class.ancestors.each do |klass|
        # If we've reached up the object heirarchy all the way to Objekt, then
        # there are no valid containers configured for this obj.class or any of
        # its ancestors.
        raise ArgumentError.new "No #{obj.class.name} container configured on #{self.class.name}" if klass == Objekt

        # Construct a container name from the klass.
        container_name = klass.name.split("::").last.downcase.to_s + "s"

        # If the Objekct responds to the container name, then we've found the
        # most-specific container configured to accept objects of this type.
        break if respond_to? container_name
      end

      send container_name
    end

  end

end
