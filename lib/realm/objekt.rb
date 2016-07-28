###############################################################################
# To fully subclass Objekt and take advantage of all its functionality, you
# SHOULD do the following:
#
# 1. Your subclass initialize method should call super before doing anything
#    else. If you pass an id to super, it will be used as the globally unique
#    identifier for the instance. If you do not pass an id to super, Objekt
#    will automatically assign a new UUIDv4 to the id attribute.
#
# 2. To take advantage of saving/loading through the Scribe, you should
#    implement #hashify and #dehashify methods. #hashify takes no parameters
#    and returns a Hash suitable for serialization (such as to YAML). The
#    #dehashify method takes one Hash as a parameter, and you should consume
#    these Hash keys/values to initialize your instance to the desired state.
###############################################################################
require "securerandom"
require "yaml"

module Realm

  class Objekt
    attr_reader :id
    attr_accessor :name

    ###########################################################################
    def initialize(id = nil)
      @id   = id || SecureRandom.uuid
      @name = "_name"
    end

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to send `@things << obj`
    def insert(obj)
      container = obj.class.name.split("::").last.downcase.to_s + "s"
      raise ArgumentError.new "Cannot insert object of type #{obj.class.name}" unless respond_to? container
      send(container) << obj
    end
    alias_method :<<, :insert

    ###########################################################################
    def serialize
      raise NotImplementedError.new "#{self.class}#hashify is not implemented" unless respond_to? :hashify
      YAML.dump base_hashify
    end

    ###########################################################################
    def base_hashify
      {
        objekt_class: self.class.name,
        objekt_id: @id,
        objekt_name: @name,
      }.deep_merge! hashify
    end

    ###########################################################################
    def self.deserialize(yaml)
      base_dehashify YAML.load yaml
    end

    ###########################################################################
    def self.base_dehashify(hash)
      # Instantiate an Object of the appropriate class.
      objekt = Object.const_get(hash[:objekt_class]).new hash[:objekt_id]
      # Load Objekt-specific attributes.
      objekt.name = hash[:objekt_name]
      # Load subclass attributes.
      objekt.dehashify hash
      objekt
    end

  end

end
