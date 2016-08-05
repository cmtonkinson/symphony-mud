module Command

  ##################################################################################################
  define :help do
    usage
    usage :command
    exec do
      if command
        cmd = ::Command.configuration.command_registry.find do |c|
          c.get_name == command.name
        end

        if cmd.nil?
          being.puts "No command '#{command.name}'."
          break
        end

        being.puts "Help for {W#{cmd.get_name}{x: #{cmd.get_brief}"
        being.puts "{YUsage:{x"
        cmd.usages.reverse.each do |arg|
          being.puts "  #{cmd.get_name} " + arg.data.map(&:name).join(" ")
        end
        if $help[:command].key? cmd.get_name
          being.puts "\n{YDescription:{x"
          being.puts $help[:command][cmd.get_name]
        else
          message = "No description is available for the '#{cmd.get_name}' command."
          being.puts "\n#{message}"
          Symphony.logger.debug message
        end
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
      command_set = being.user.operators.peek.command_set
      being.puts command_set.map(&:get_name).join ", "
    end
  end

  ##################################################################################################
  define :quit do
    usage
    exec do
      being.puts I18n.t("quit")
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
