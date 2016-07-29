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
#    implement #_hashify and #_dehashify methods. #_hashify takes no parameters
#    and returns a Hash suitable for serialization (such as to YAML). The
#    #dehashify method takes one Hash as a parameter, and you should consume
#    these Hash keys/values to initialize your instance to the desired state.
#
# 3. To enable smart deep copying via the #clone method, your subclass must
#    implement a #_deep_copy method.
###############################################################################
require "securerandom"
require "set"
require "yaml"

module Realm

  class Objekt

    DEFAULTS = {
      id: -> { SecureRandom.uuid },
      name: "[name]",
    }

    attr_reader :id, :cloned_from
    attr_accessor :name, :containers
    attr_accessor :_disposal_requested, :_disposal_check_visits

    ###########################################################################
    def initialize(id = nil)
      # Each Objekt must have a globally unique identifier. By default, a UUID
      # (version 4) is generated automatically, but an id can be explicitly
      # passed to override this behavior. If an id is passed manually, it MUST
      # be globally unique or Bad Things™ will happen.
      @id = id || DEFAULTS[:id].call

      # When #clone is called on an Objekt, a new Objekt is returned with the
      # exact properties of the original, except two: the new Objekt has its
      # own id, and the cloned_from property will be set to the id of the
      # original.
      @cloned_from = nil

      # The name attribute is more descriptive than anything else. At this time
      # its use is not strictly defined by the Objekt class but it should
      # contain a string that makes sense in the context of whatever your
      # subclass is.
      @name = DEFAULTS[:name].dup

      # By adding `Util::AttrHash` instance members to your subclasses, you
      # can easily keep collections of Objekts organized. If you decide to
      # use the Objekt methods #insert and #remove as abstractions over these
      # containers, Objekt will automatically keep track of which containers
      # your Objekt is held in. In other words, the #insert and #remove helpers
      # will bookkeep the bidirectional association between a container and
      # each Objekt in it.
      @containers = Set.new

      # Every Objekt created stores itself in the Registry singleton:
      # 1. For various bookkeeping tasks
      # 2. For runtime performance analysis
      # 3. So we can easily perform global id-based lookups (e.g. finding a
      #    Room by its id without needing to iterate all Zones)
      Util::Registry << self

      # Since all Objekts get referenced in the Registry, any client code that
      # wishes to signify an Objekt can be GC'd must call its #dispose method
      # and immediately release its remaining reference. The following members
      # which aid in the bookkeeping required for this should be considered
      # private and not manipulated by client code.
      @_disposal_requested    = false
      @_disposal_check_visits = 0
    end

    ###########################################################################
    def dispose
      @_disposal_requested = true
    end

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to call `@things.insert obj`
    def insert(obj)
      container = container_for obj
      container.insert obj
      obj.containers.add self
    end
    alias_method :<<, :insert

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to call `@things.remove obj`
    def remove(obj)
      container = container_for obj
      container.remove obj
      obj.containers.delete self
    end
    alias_method :-, :insert

    ###########################################################################
    def container_for(obj)
      container = container_name obj.class
      raise ArgumentError.new "No #{obj.class.name} container configured on #{self.class.name}" unless respond_to? container
      send container
    end

    ###########################################################################
    def container_name(klass)
      klass.name.split("::").last.downcase.to_s + "s"
    end

    ###########################################################################
    def clone
      raise NotImplementedError.new "#{self.class.name}#clone called but #deep_copy is not implemented" unless respond_to? :deep_copy
      # Generate a new subclass-type Objekt, setting the cloned_from id.
      dolly = self.class.new
      dolly.instance_variable_set :@cloned_from, id
      # Deep copy Objekct properties.
      deep_copy dolly
      # Deep copy subclass properties.
      _deep_copy dolly
      # Return the clone.
      dolly
    end

    ###########################################################################
    def deep_copy(other)
      other.name = name.dup
    end

    ###########################################################################
    def serialize
      raise NotImplementedError.new "#{self.class.name}#serialize called but #hashify is not implemented" unless respond_to? :hashify
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
      # Return the completed objekt.
      objekt
    end

  end

end
