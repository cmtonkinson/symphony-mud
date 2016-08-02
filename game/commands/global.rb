module Command

  ##################################################################################################
  define :help do
    usage
    usage :command
    exec do
      if command
        user.puts "Executed HELP for command '#{command}'"
      else
        user.puts "Executed HELP"
      end
    end
  end

  ##################################################################################################
  define :commands do
    usage
    exec do
      user.puts "The following commands are available:"
      user.puts "TODO iterate over User.operators"
    end
  end

  ##################################################################################################
  define :quit do
    usage
    exec do
      user.puts "Logging out. Goodbye."
      user.client.should_terminate = true
    end
  end

  ##################################################################################################
  define :say do
    usage :rest
    exec do
      user.puts "You say, \"#{rest}\""
    end
  end

  ##################################################################################################
  define :look do
    usage
    exec do
      user.puts "You look around."
    end
  end

  ##################################################################################################
#  Realm::Exit::DIRECTIONS.keys.each do |direction|
#    define direction do
#      usage
#      exec do
#        msg = being.can_move?(direction)
#        if msg.is_a? String
#          user.puts msg
#        else
#          being.move direction
#        end
#      end
#    end
#  end

end
