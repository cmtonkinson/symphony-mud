module Command

# TODO - support aliases (esp. for command shortcuts like NESWUD)
# TODO - set default aliases (NESWUP, kill, etc.)

###############################################################################
define :help do
###############################################################################
  usage :command
  exec do
    if command
      cmd = ::Command.configuration.command_registry.find do |c|
        c.get_name == command.name
      end
      being.puts I18n.t("help.not_found"), command.name and next if cmd.nil?

      name  = cmd.get_name
      brief = cmd.get_brief
      usage = Util::Helper.print_usages_for cmd

      desc = if $help[:command].key?(cmd.get_name)
        $help[:command][cmd.get_name]
      else
        I18n.t("help.no_description", cmd: cmd.get_name)
      end

      being.puts $templates.help, name, brief, usage, desc
    end
  end
end

###############################################################################
define :commands do
###############################################################################
  usage
  exec do
    being.puts I18n.t("commands")
    command_set = being.user.operators.peek.command_set
    being.puts command_set.map(&:get_name).join ", "
  end
end

###############################################################################
define :quit do
###############################################################################
  usage
  exec do
    being.puts I18n.t("quit")
    being.user.client.should_terminate = true
  end
end

###############################################################################
define :say do
###############################################################################
  usage :rest
  exec do
    being.puts I18n.t("say.you_say"), rest
    room.puts I18n.t("say.says"), being, rest, exclude: being
  end
end

###############################################################################
define :summary do
###############################################################################
  usage
  exec do
    being.puts I18n.t("summary.name") + " " + I18n.t("objekt.identify", being.identify)
  end
end

###############################################################################
define :look do
###############################################################################
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

Realm::Exit::DIRECTIONS.keys.each do |direction|
###############################################################################
  define direction do
###############################################################################
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
