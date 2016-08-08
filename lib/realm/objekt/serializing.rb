require "yaml"

module Realm
  class Objekt

    ###########################################################################
    def serialize
      YAML.dump hashify
    end

    ###########################################################################
    def hashify
      # Create a base Hash.
      hash = {
        objekt_class: self.class.name,
        objekt_id: @id,
        objekt_name: @name,
      }
      # Run the hook to get other Hashes from different functional areas.
      sub_hashes = run_hook :on_hashify
      # Merge all the subsequent Hashes into the original.
      sub_hashes.each { |s| hash.deep_merge s }
      # Return the merged result.
      hash
    end

    ###########################################################################
    def self.deserialize(yaml)
      dehashify YAML.load yaml
    end

    ###########################################################################
    def self.dehashify(hash)
      # Instantiate an Object of the appropriate class.
      objekt = Object.const_get(hash[:objekt_class]).new hash[:objekt_id], name: hash[:objekt_name]
      # Run dehashification hooks to complete the loading.
      objekt.run_hook :on_dehashify, hash
      # Return the completed objekt.
      objekt
    end

  end
end
