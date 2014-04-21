module Command

  # ##################################################################################################
  # # help
  # ##################################################################################################
  define :help do
    level 1
    exec do
      @user.puts "Executed HELP command."
    end
  end
  # class Command_help < Command
  #   def initialize
  #     name :help
  #     exec do
  #       @user.puts "Executed HELP command."
  #     end
  #   end
  # end

  ##################################################################################################
  # say
  ##################################################################################################
  define :say do
    exec do
      @user.puts "You say, \"#{@arg_string}\""
    end
  end
  # class Command_say < Command
  #   def initialize
  #     name :say
  #     exec do
  #       @user.puts "You say, \"#{@arg_string}\""
  #     end
  #   end
  # end

  ##################################################################################################
  # shutdown
  ##################################################################################################
  class Command_shutdown < Command
    def initialize
      name = :shutdown
      exec do
        @user.puts "Shutting down the server."
        $universe.should_stop = true
      end
    end
  end

  ##################################################################################################
  # quit
  ##################################################################################################
  define :quit do
    exec do
      @user.client.should_terminate = true
    end
  end
  class Command_quit < Command
    def initialize
      name = :quit
      exec do
        @user.puts "Logging out. Goodbye."
        @user.client.should_terminate = true
      end
    end
  end

  ##################################################################################################
  # reboot
  ##################################################################################################
  class Command_reboot < Command
    def initialize
      name = :reboot
      exec do
        @user.puts "Hold on to your butts.... Copyover!!"
        $universe.should_reboot = true
      end
    end
  end

end
