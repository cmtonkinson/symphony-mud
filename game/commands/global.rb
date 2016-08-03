module Command

  ##################################################################################################
  define :help do
    usage
    usage :command
    exec do
      if command
        being.puts "Executed HELP for command '#{command}'"
      else
        being.puts "Executed HELP"
      end
    end
  end

  ##################################################################################################
  define :commands do
    usage
    exec do
      being.puts "The following commands are available:"
      being.puts "TODO iterate over User.operators"
    end
  end

  ##################################################################################################
  define :quit do
    usage
    exec do
      being.puts "Logging out. Goodbye."
      user.client.should_terminate = true
    end
  end

  ##################################################################################################
  define :say do
    usage :rest
    exec do
      being.puts "You say, \"#{rest}\""
    end
  end

  ##################################################################################################
  define :summary do
    usage
    exec do
      being.puts I18n.t("summary.name") + " " + I18n.t("objekt.identify", being.identify)
    end
  end

  ##################################################################################################
  define :look do
    usage
    exec do
      exits = Realm::Exit::DIRECTIONS
        .keys
        .map { |dir| I18n.t("movement.exits.#{dir}") if being.room.send dir }
        .compact
        .join " "

      being.puts I18n.t "objekt.identify", room.identify
      being.puts I18n.t "look.exit_list", exits: exits
    end
  end

  ##################################################################################################
  Realm::Exit::DIRECTIONS.keys.each do |direction|
    define direction do
      usage
      exec do
        msg = being.can_move? direction
        if msg === true
          being.move direction
        else
          being.puts msg
        end
      end
    end
  end

end
