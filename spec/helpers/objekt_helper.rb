module Realm

  class TestObj < Objekt

    attr_accessor :my_num

    ###########################################################################
    def initialize(id = nil)
      super id
    end

    ###########################################################################
    def hashify
      {
        my_num: 456
      }
    end

    ###########################################################################
    def dehashify(hash)
      @my_num = hash[:my_num]
    end

  end

end
