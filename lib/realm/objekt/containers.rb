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
    alias_method :<<, :insert

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to call `@things.remove obj`
    def remove(objekt)
      container = container_for objekt
      container.remove objekt
      objekt.containers.delete self
    end
    alias_method :-, :insert

    ###########################################################################
    def container_for(obj)
      container = obj.class.name.split("::").last.downcase.to_s + "s"
      raise ArgumentError.new "No #{obj.class.name} container configured on #{self.class.name}" unless respond_to? container
      send container
    end

  end

end
