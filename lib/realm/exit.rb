module Realm

  class Exit < Objekt

    simple_flag :open, true, :open?, :open!, :closed?, :close!
    simple_flag :lock, false, :locked?, :lock!, :unlocked?, :unlock!

    ###########################################################################
    def initialize(id = nil)
      super id
      open!
      unlock!
    end

    ###########################################################################
    def _hashify
      {}
    end

    ###########################################################################
    def _dehashify(hash)
    end

  end

end
