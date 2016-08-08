module Realm
  class Objekt

    include Hooks

    # Unless otherwise noted, all Objekt hooks run in the context of an Objekt
    # instance.

    # after_initialize is invoked at the end of Objekt#initialize.
    define_hook :after_initialize

    # before_disposal is invoked at the beginninf of Objekt#dispose.
    define_hook :before_disposal

    # on_hashify is invoked when serializing an Objekt. It should return a Hash
    # object including any relevant key/value pairs that you wish to have
    # persisted to disk for the Objekt.
    define_hook :on_hashify
    # on_dehashify is invoked when derserializing an Objekt. It takes a Hash
    # object and should include the key/value pairs you defined in on_hashify
    # so that you may reinitialize your Objekt accordingly.
    define_hook :on_dehashify

  end
end
