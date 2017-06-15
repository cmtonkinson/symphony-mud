module Command

# TODO - support aliases (esp. for command shortcuts like NESWUD)
# TODO - set default aliases (NESWUP, kill, etc.)

###############################################################################
Realm::Exit::DIRECTIONS.keys.each do |direction| define direction do
###############################################################################
  shortcut direction[0]
  usage
  exec do
    msg = being.can_go? direction
    if msg === true
      being.go direction
    else
      being.puts msg
    end
  end
end; end

###############################################################################
define :colors do
###############################################################################
  usage
  exec do
    being.puts t("title")
    Base::Terminal::COLORS.each do |name,info|
      being.puts "  {{#{info[:code]} = {#{info[:code]}#{name}{x"
    end
    being.puts t("reset")
    being.puts "  {{x = clear/default"
    being.puts t("bracket")
    being.puts "  {{{{ = {"
  end
end

###############################################################################
define :commands do
###############################################################################
  usage
  exec do
    being.user.operators.each do |operator|
      command_set       = operator.command_set
      commands_by_level = command_set.group_by &:get_level

      being.puts t("actor"), operator.set_name
      Realm::Being::LEVELS.reverse.each do |level_name|
        level_num = Realm::Being.const_get level_name
        next unless being.level >= level_num
        next unless commands_by_level.key? level_num

        being.puts "\n  --== {Y#{level_name}{x (#{commands_by_level[level_num].length} commands) ==--"
        being.puts "  " + commands_by_level[level_num].map(&:get_name).join(", ")
      end
    end
  end
end

###############################################################################
define :drop do
###############################################################################
  usage(items: :id)
  exec do
    targets = being.inventory.locate items
    being.puts t("err_not_found") and next if targets.empty?

    targets.each do |item|
      item.move.from(being.inventory).to(room.inventory)
      being.puts t("actor"), item
      room.puts t("other"), being, item, exclude: being
    end
  end
end

###############################################################################
define :emote do
###############################################################################
  usage :rest
  exec do
    room.puts rest.prepend("$being "), being
  end
end

###############################################################################
define :get do
###############################################################################
  usage items: :id
  exec do
    targets = room.inventory.locate items
    being.puts t("err_not_found") and next if targets.empty?

    targets.each do |item|
      item.move.from(room.inventory).to(being.inventory)
      being.puts t("actor"), item
      room.puts t("other"), being, item, exclude: being
    end
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
      being.puts t("err_not_found"), command.name and next if cmd.nil?

      name  = cmd.get_name
      brief = cmd.get_brief
      usage = Util::Helper.print_usages_for cmd

      desc = if $help[:command].key?(cmd.get_name)
        $help[:command][cmd.get_name]
      else
        t "err_no_desc"
      end

      being.puts $templates.help, name, brief, usage, desc
    end
  end
end

###############################################################################
define :inventory do
###############################################################################
  shortcut "i"
  usage
  exec do
    being.puts t("title"), being.inventory.display_contents
  end
end

###############################################################################
define :kill do
###############################################################################
  shortcut "k"
  usage({victim: :id})
  exec do
    targets = room.beings.locate victim
    being.puts t("err_noone") and next if targets.empty?
    being.puts t("err_too_many") and next if targets.length > 1
    target = targets.first

    being.puts t("title"), target
    Combat.embattle being, target
  end
end

###############################################################################
define :look do
###############################################################################
  shortcut "l"
  usage
  exec do
    exits = Realm::Exit::DIRECTIONS
      .keys
      .map { |dir| I18n.t("movement.exit.#{dir}") if being.room.send dir }
      .compact
      .join " "

    being.puts t("title"), room, room
    being.puts t("exit_list"), exits

    being.other_beings.each { |b| being.puts t("here"), b }

    next if room.inventory.empty?
    being.puts t("item_list"), room.inventory.display_contents
  end
end

###############################################################################
# TODO implement "exploration" where players don't see Rooms mapped until they
# have visited them. Could link up to achievements, exp bonuses, show up on
# the summary page, etc.
define :map do
###############################################################################
  usage
  exec do
    zone   = being.room.zone
    layout = zone.get_layout
    height = layout.length
    width  = layout[0].length
    buffer = ""

    being.puts t("title"), zone

    layout.reverse.each_with_index do |row,row_index|
      row.each_with_index do |point,col_index|
        buffer << case point
        when Realm::Room
          room == point ? "{C@{x" : "x"
        when Realm::Exit
          point.north.nil? ? "-" : "|"
        else
          " "
        end
      end
      buffer << "\n"
    end

    being.puts buffer
  end
end

###############################################################################
define :quit do
###############################################################################
  usage
  exec do
    being.cmd "save"
    being.puts t("actor")
    being.user.client.should_terminate = true
  end
end

###############################################################################
define :save do
###############################################################################
  usage
  exec do
    being.print t("start")
    Logistics::Scribe.save being
    being.puts t("success")
  end
end

###############################################################################
define :say do
###############################################################################
  usage :rest
  exec do
    being.puts t("actor"), rest
    room.puts t("hear"), being, rest, exclude: being
  end
end

###############################################################################
define :shortcuts do
###############################################################################
  usage
  exec do
    commands = []
    ::Command.configuration.command_sets.values.each { |set| commands << set.to_a }

    list = commands.flatten
      .reject { |c| c.get_shortcut.nil? }
      .select { |c| being.level >= (c.level || 0) }
      .sort_by { |c| c.get_shortcut }
      .map { |c| "  %s - %s" % [c.get_shortcut, c.get_name] }
   
    being.puts t("title") 
    being.puts list.join "\n"
 end
end

###############################################################################
define :summary do
###############################################################################
  usage
  exec do
    being.puts t("title"), being.name, being.level_label

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
      being.inventory.length, 0, 0, 0,
      0, 0, 0
    ]
  end
end

###############################################################################
define :who do
###############################################################################
  usage
  exec do
    being.puts t("title")
    $world.avatars.values.each do |a|
      being.puts "  " + a.name
    end
  end
end

###############################################################################
define :whoami do
###############################################################################
  usage
  exec do
    being.puts t("actor"), being
  end
end

###############################################################################
define :zones do
###############################################################################
  usage
  exec do
    being.puts t("title")
    $world.zones.values.sort_by(&:rid).each do |zone|
      next if zone.rid == 1 && !being.demigod?
      being.puts t("line"), zone, zone, zone.rooms.length, zone.items.length, zone.npcs.length
    end
  end
end

end
