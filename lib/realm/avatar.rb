module Realm

  class Avatar < Being

    attr_accessor :user

    ###########################################################################
    def initialize(id = nil, name = nil)
      super id
      @name = name
    end

    ###########################################################################
    # Proxy the following methods to the User object.
    %i(print puts).each do |meth|
      define_method meth do |message|
        @user.send meth, message
      end
    end

  end

end
