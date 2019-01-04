require 'yaml'

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
        objekt: {
          class: self.class.name,
          id: @id,
          rid: @rid,
          name: @name,
        },
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
      o_hash = hash[:objekt]
      # Instantiate an Object of the appropriate class.
      objekt = Object.const_get(o_hash[:class]).new(o_hash[:id], {
                                                      rid: o_hash[:rid],
                                                      name: o_hash[:name],
                                                    })
      # Run dehashification hooks to complete the loading.
      objekt.run_hook :on_dehashify, hash
      # Return the completed Objekt.
      objekt
    end
  end
end
