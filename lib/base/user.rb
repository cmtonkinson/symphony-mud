module Base

  class User
    attr_accessor :client, :avatar, :operators

    ###########################################################################
    def initialize
      @client    = nil
      @avatar    = nil
      @operators = Util::Stack.new
    end

    ###########################################################################
    def bind(client)
      @client = client
      @client.user = self
      operators.push Command::Operator.new self
      operators.push InputSequence::Controller.new self, InputSequence::LoginSequence.new
    end

    ###########################################################################
    # Proxy Client methods.
    %i(print puts).each do |meth|
      define_method meth do |message|
        client.send meth, message
      end
    end

  end

end
