require 'json'

module Command

  ##################################################################################################
  # help
  ##################################################################################################
  class Command_help < Command
    def initialize
      @name = "help"
    end
    def execute(user, arguments)
      user.puts "You are now being helped. You are welcome."
      user.puts "Arguments: #{arguments.to_json}"
    end
  end

  ##################################################################################################
  # shutdown
  ##################################################################################################
  class Command_shutdown < Command
    def initialize
      @name = "shutdown"
    end
    def execute(user, arguments)
      user.puts "Shutting down."
      $universe.should_stop = true
    end
  end

  ##################################################################################################
  # quit
  ##################################################################################################
  class Command_quit < Command
    def initialize
      @name = "quit"
    end
    def execute(user, arguments)
      user.puts "Goodbye."
      user.client.should_terminate = true
    end
  end

  ##################################################################################################
  # reboot
  ##################################################################################################
  class Command_reboot < Command
    def initialize
      @name = "reboot"
    end
    def execute(user, arguments)
      user.puts "=== Hold on to your butts.... Copyover NOW"
      $universe.should_reboot = true
    end
  end

end
