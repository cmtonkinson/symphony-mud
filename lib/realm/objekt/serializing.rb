require "yaml"

module Realm

  class Objekt

    ###########################################################################
    def serialize
      raise NotImplementedError.new "#{self.class.name}#serialize called but #_hashify is not implemented" unless respond_to? :_hashify
      YAML.dump hashify
    end

    ###########################################################################
    def hashify
      {
        objekt_class: self.class.name,
        objekt_id: @id,
        objekt_name: @name,
      }.deep_merge! _hashify
    end

    ###########################################################################
    def self.deserialize(yaml)
      dehashify YAML.load yaml
    end

    ###########################################################################
    def self.dehashify(hash)
      # Instantiate an Object of the appropriate class.
      objekt = Object.const_get(hash[:objekt_class]).new hash[:objekt_id]
      # Load Objekt-specific attributes.
      objekt.name = hash[:objekt_name]
      # Load subclass attributes.
      objekt._dehashify hash
      # Return the completed objekt.
      objekt
    end

  end

end
