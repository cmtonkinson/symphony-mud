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
      self.client = client
      self.client.user = self
      operators.push Command::Operator.new self, Command.configuration.command_sets[:global]
      operators.push InputSequence::Controller.new self, InputSequence::LoginSequence.new
    end

    ###########################################################################
    def print(message)
      client.print message
    end

    ###########################################################################
    def puts(message)
      client.puts message
    end

  end

end
