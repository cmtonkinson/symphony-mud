module Realm

  class TestObj < Objekt

    attr_accessor :my_num

    ###########################################################################
    def initialize(id = nil)
      super id
    end

    ###########################################################################
    def _hashify
      {
        my_num: 456
      }
    end

    ###########################################################################
    def _dehashify(hash)
      @my_num = hash[:my_num]
    end

    ###########################################################################
    def _deep_copy(other)
      other.my_num = @my_num
    end

  end

end
