module Realm

  class TestObj < Objekt

    attr_accessor :my_num

    def initialize(id = nil)
      super
    end

    ###########################################################################
    # For testing serialization.
    ###########################################################################
    def _hashify
      {
        my_num: 456
      }
    end

    def _dehashify(hash)
      @my_num = hash[:my_num]
    end

    ###########################################################################
    # For testing deep Objekt copies.
    ###########################################################################
    def _deep_copy(other)
      other.my_num = @my_num
    end

  end

  ###########################################################################
  # For testing inheritance-friendly containers.
  ###########################################################################
  class TestBucket < Objekt
    attr_accessor :testparents
    def initialize(id = nil)
      super
      @testparents = Util::AttrHash.new
    end
  end

  class TestParent < Objekt
    def initialize(id = nil)
      super
    end
  end

  class TestChild < TestParent
    def initialize(id = nil)
      super
    end
  end

end
