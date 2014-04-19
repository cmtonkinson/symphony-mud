require 'json'

module Command

  class Command_help < Command
    def initialize
      @name = "help"
    end

    def execute(client, arguments)
      client.puts "You are now being helped. You are welcome."
      client.puts "Arguments: #{arguments.to_json}"
    end
  end

end
