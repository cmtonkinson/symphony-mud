module Command

  class Operator

     def initialize(client, command_set)
      @state       = {}
      @last_input  = ""
      @client      = client
      @command_set = command_set
    end

    def handle(input)
      command = @command_set.find { |c| c.name == input }
      return if command.nil?
      command.execute @client, nil
    end

  end

end
