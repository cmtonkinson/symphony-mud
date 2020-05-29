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
#
# 4. To enable the Lingust class to smartly print an Objekt, you need to
#    supply the correct formatting hint by implementing a subclass method
#    called _interpret_name and returning a String (e.g. "$item").
###############################################################################
require 'securerandom'

module Realm
  class Objekt
    # Defining Objekt property defaults at the class-level like this makes
    # certain meta-operations easier, such as for testing.
    DEFAULTS = {
      id: -> { SecureRandom.uuid },
      rid: -> { nil },
      name: -> { +'[name]' },
    }

    attr_reader :id
    attr_accessor :rid, :name
    attr_accessor :_disposal_requested, :_disposal_check_visits

    ###########################################################################
    def initialize(id = nil, props = {})
      # Each Objekt must have a globally unique identifier. By default, a UUID
      # (version 4) is generated automatically, but an id can be explicitly
      # passed to override this behavior. If an id is passed manually, it MUST
      # be globally unique or Bad Things(tm) will happen.
      @id = id || DEFAULTS[:id].call

      # Objekts may be assigned a "Realm Identifier" (`rid`). The RID is a
      # superficial field and has no meaning to the Objekt class itself; the
      # Objekt class only defines the field, accessors and hashify/dehashify
      # hooks, therefore the RID itself must be some data type natively JSON
      # serializable. It is intended primarily for use as a more user-friendly
      # alternative to UUIDs for circumstances where users will need to
      # manipulate various objects by some simple deterministic label,
      # analogous to the way vnums are manipulated by admins and builders in
      # ROM-based games. However, the actual use (or non-use) of the RID is
      # left as an implementation detail of each individual Objekt subclass.
      @rid = DEFAULTS[:rid].call

      # The name attribute is more descriptive than anything else. At this time
      # its use is not strictly defined by the Objekt class but it should
      # contain a string that makes sense in the context of whatever your
      # subclass is.
      @name = DEFAULTS[:name].call

      # Allow setting custom attributes on the Objekt at initialization time by
      # passing a Hash of attributes and values.
      props.each { |key, val| instance_variable_set "@#{key}", val }

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
    def fully_qualified_identifier
      {
        klass: self.class.name.split('::').last,
        name: @name,
        id: @id[0..7],
        rid: @rid,
      }
    end
    alias_method :fqid, :fully_qualified_identifier

    ###########################################################################
    def fully_qualified_identifier_string
      I18n.translate 'objekt.identify', **self.fully_qualified_identifier
    end
    alias_method :fqid_str, :fully_qualified_identifier_string

    ###########################################################################
    def interpret_name
      _interpret_name
    end
  end
end
