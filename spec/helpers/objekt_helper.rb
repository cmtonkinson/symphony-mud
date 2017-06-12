module Realm

  class TestObj < Objekt

    attr_accessor :my_num, :my_nil

    attr_reader :before_remove_ran, :after_remove_ran, :before_add_ran, :after_add_ran

    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    # For testing serialization.
    on_hashify -> {
      {
        my_num: 456
      }
    }

    on_dehashify -> hash {
      @my_num = hash[:my_num]
    }

    ###########################################################################
    # For testing deep Objekt copies.
    on_deep_copy -> other {
      @my_num = other.my_num
      @my_nil = other.my_nil
    }

    ###########################################################################
    # For testing add/remove hooks
    before_remove -> { @before_remove_ran = true }
    after_remove  -> { @after_remove_ran  = true }
    before_add    -> { @before_add_ran    = true }
    after_add     -> { @after_add_ran     = true }

  end

  ###########################################################################
  # For testing inheritance-friendly containers.
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
