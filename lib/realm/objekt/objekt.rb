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

module Realm
  class Objekt

    # Defining Objekt property defaults at the class-level like this makes
    # certain meta-operations easier, such as for testing.
    DEFAULTS = {
      id: -> { SecureRandom.uuid },
      name: "[name]",
    }

    attr_reader :id
    attr_accessor :name
    attr_accessor :_disposal_requested, :_disposal_check_visits

    ###########################################################################
    def initialize(id = nil, props = {})
      # Each Objekt must have a globally unique identifier. By default, a UUID
      # (version 4) is generated automatically, but an id can be explicitly
      # passed to override this behavior. If an id is passed manually, it MUST
      # be globally unique or Bad Things(tm) will happen.
      @id = id || DEFAULTS[:id].call

      # The name attribute is more descriptive than anything else. At this time
      # its use is not strictly defined by the Objekt class but it should
      # contain a string that makes sense in the context of whatever your
      # subclass is.
      @name = DEFAULTS[:name].dup

      # Allow setting custom attributes on the Objekt at initialization time by
      # passing a Hash of attributes and values.
      props.each { |key,val| instance_variable_set "@#{key}", val }

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

      # Allow subclasses to define custom logic after Objekt initialization.
      self.run_hook :after_initialize
    end

    ###########################################################################
    def dispose
      self.run_hook :before_disposal
      @_disposal_requested = true
    end

    ###########################################################################
    def disposed?
      @_disposal_requested
    end

    ###########################################################################
    def identify
      {name: @name, id: @id}
    end

  end
end
