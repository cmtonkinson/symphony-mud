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
      being.user.client.should_terminate = true
    end
  end

  ##################################################################################################
  define :say do
    usage :rest
    exec do
      being.puts I18n.t("say.you_say"), rest
      room.puts I18n.t("say.says"), being, rest, exclude: being
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
        .map { |dir| I18n.t("movement.exit.#{dir}") if being.room.send dir }
        .compact
        .join " "

      being.puts I18n.t "objekt.identify", room.identify
      being.puts I18n.t "look.exit_list", exits: exits

      being.other_beings.each { |b| being.puts I18n.t("look.is_here"), b }
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
