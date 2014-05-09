module Command

  ##################################################################################################
  # help
  ##################################################################################################
  define :help do
    format
    format :command
    exec do
      user.puts "Executed HELP command."
    end
  end

  ##################################################################################################
  # quit
  ##################################################################################################
  define :quit do
    exec do
      user.puts "Logging out. Goodbye."
      user.client.should_terminate = true
    end
  end

  ##################################################################################################
  # say
  ##################################################################################################
  define :say do
    exec do
      user.puts "You say, \"#{@arg_string}\""
    end
  end

end
