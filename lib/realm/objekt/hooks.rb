module Realm
  class Objekt
    include Hooks
    include Hooks::InstanceHooks

    # Unless otherwise noted, all Objekt hooks run in the context of an Objekt
    # instance.

    # after_initialize is invoked at the end of Objekt#initialize.
    define_hook :after_initialize

    # before_disposal is invoked at the beginning of Objekt#dispose.
    define_hook :before_disposal

    # on_hashify is invoked when serializing an Objekt. It should return a Hash
    # object including any relevant key/value pairs that you wish to have
    # persisted to disk for the Objekt.
    define_hook :on_hashify
    # on_dehashify is invoked when derserializing an Objekt. It takes a Hash
    # object and should include the key/value pairs you defined in on_hashify
    # so that you may reinitialize your Objekt accordingly.
    define_hook :on_dehashify

    # on_deep_copy is invoked when cloning an Objekt. It takes an Objekt
    # reference (which is the object being cloned from) and the callback runs
    # in the context of the clone.
    define_hook :on_deep_copy

    # vivify is invoked when an Objekt is stitched into the World. For an
    # Avatar, that could be as a final step of logging in. For an Item, that
    # could (for example) be as a final step of being loaded by a placement.
    define_hook :vivify

    # enervate is the inverse of the :vivify hook, invoked when an Objekt is
    # being removed from the World. It is related in nature to the Objekt
    # disposal process, but whereas Objekt#dispose is a low-level mechanical
    # operation, the enevrate hook is a game-level operation to perform work.
    define_hook :enervate
  end
end
