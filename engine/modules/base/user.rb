module Base

  class User
    attr_accessor :client, :operators

    def initialize
      @client    = nil
      @operators = Util::Stack.new
    end

    def bind(client)
      self.client = client
      client.user = self
      self.operators.push Command::Operator.new self, Command.configuration.command_sets[:global]
    end

    def puts(message)
      self.client.puts message if self.client
    end

  end

end
