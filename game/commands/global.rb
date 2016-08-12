module Command

# TODO - support aliases (esp. for command shortcuts like NESWUD)
# TODO - set default aliases (NESWUP, kill, etc.)

###############################################################################
Realm::Exit::DIRECTIONS.keys.each do |direction| define direction do
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
end; end

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

###############################################################################
define :quit do
###############################################################################
  usage
  exec do
    being.cmd "save"
    being.puts I18n.t("quit")
    being.user.client.should_terminate = true
  end
end

###############################################################################
define :rooms do
###############################################################################
  usage
  usage({id: :int})
  brief "List the Rooms in the given Zone (or the current Zone if unspecified)"
  exec do
    zone = id.nil? ? being.room.zone : $world.zones.values.find { |z| z.rid == id }

    if zone.nil?
      being.puts "Couldn't find that Zone."
      next
    end

    being.puts "Rooms in {m%s{x:\n\n" % zone.name
    zone.rooms.each do |id,room|
      being.puts "[{C%3u{x] {M%s{x" % [room.rid, room.name]
    end
  end
end

###############################################################################
define :save do
###############################################################################
  usage
  exec do
    being.print "Saving your profile..."
    Realm::Scribe.save being
    being.puts " done!"
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
    being.puts I18n.t("summary.name", name: being.name)
    being.puts I18n.t("objekt.identify", being.identify) # TODO hide unless admin

    # The arguments below for sprintf follow the summary template line by line.
    being.puts $templates.summary % [
      "race", being.strength.current, being.hit_bonus.current, being.level,
      "class", being.dexterity.current, being.damage_bonus.current, being.health.current, being.health.max,
      being.gender, being.constitution.current, being.mana.current, being.mana.max,
      0, being.intelligence.current, being.armor.current, being.stamina.current,
      "hand", being.focus.current, being.bash.current, being.exp,
      "height", being.creativity.current, being.slash.current, being.tnl,
      "weight", being.charisma.current, being.pierce.current, 0,
      "totem", being.luck.current, being.exotic.current,
      0, 0, 0, 0,
      0, 0, 0
    ]
  end
end

###############################################################################
define :who do
###############################################################################
  usage
  brief "Display a list of players online."
  exec do
    being.puts "Currently online:\n\n"
    $world.avatars.values.each do |a|
      being.puts "  " + a.name
    end
  end
end

###############################################################################
define :zones do
###############################################################################
  usage
  brief "List the different zones in the Realm."
  exec do
    being.puts "Zones in the realm:\n\n"
    $world.zones.each do |id,zone|
      being.puts "[{C%3u{x] {m%s{x (%u rooms)" % [zone.rid, zone.name, zone.rooms.length]
    end
  end
end

end
