module Command

  class Operator

     def initialize(client, command_set)
      @state       = {}
      @last_input  = ""
      @client      = client
      @command_set = command_set
    end

    def handle(input)
      name    = input.split(" ").first
      command = @command_set.find { |c| c.get_name == name }
      return if command.nil?
      command.perform @client, input
    end

  end

end
