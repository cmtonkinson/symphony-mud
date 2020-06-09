module Command
  # TODO: support aliases (esp. for command shortcuts like NESWUD)
  # TODO: set default aliases (NESWUP, kill, etc.)

  ###############################################################################
  Realm::Exit::DIRECTIONS.keys.each do |direction|
    define direction do
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
    end
  end

  ###############################################################################
  define :colors do
    usage
    exec do
      being.puts t('title')
      Base::Terminal::COLORS.each do |name, info|
        being.puts "  {{#{info[:code]} = {#{info[:code]}#{name}{x"
      end
      being.puts t('reset')
      being.puts '  {{x = clear/default'
      being.puts t('bracket')
      being.puts '  {{{{ = {'
    end
  end

  ###############################################################################
  define :commands do
    usage
    exec do
      being.user.operators.each do |operator|
        command_set       = operator.command_set
        commands_by_level = command_set.group_by(&:get_level)

        being.puts t('actor'), operator.set_name
        Realm::Being::LEVELS.reverse_each do |level_name|
          level_num = Realm::Being.const_get level_name
          next unless being.level >= level_num
          next unless commands_by_level.key? level_num

          being.puts "\n{Y#{level_name}{x"
          being.puts '  ' + commands_by_level[level_num].map(&:get_name).join(', ')
        end
      end
    end
  end

  ###############################################################################
  define :drop do
    usage(items: :id)
    exec do
      targets = being.items.locate items
      being.puts t('err_not_found') and next if targets.empty?

      targets.each do |item|
        item.move.from(being).to(room)
        being.puts t('actor'), item
        room.puts t('other'), being, item, exclude: being
      end
    end
  end

  ###############################################################################
  define :emote do
    usage :rest
    exec do
      room.puts rest.prepend('$being '), being
    end
  end

  ###############################################################################
  define :equipment do
    usage
    exec do
      being.puts t('title')
      being.anatomy.slots.each do |slot|
        spaces = ' ' * (14 - slot.name.length)
        if slot.empty?
          being.puts t('position_unoccupied'), slot.name, spaces
        else
          being.puts t('position_occupied'), slot.name, spaces, slot.occupant
        end
      end
    end
  end

  ###############################################################################
  define :get do
    usage items: :id
    exec do
      targets = room.items.locate items
      being.puts t('err_not_found') and next if targets.empty?

      targets.each do |item|
        item.move.from(room).to(being)
        being.puts t('actor'), item
        room.puts t('other'), being, item, exclude: being
      end
    end
  end

  ###############################################################################
  define :group do
    usage
    usage({ action: %w[leave disband] })
    usage({ action: %w[join withdraw accept deny kick] }, { victim: :id })
    exec do
      group  = being.group
      leader = group.leader

      if action == 'leave'
        being.puts t('err_your_group') and next if being == leader

        group.leave being
      elsif action == 'disband'
        being.puts t('err_not_leader') and next unless being == leader

        group.disband
      elsif action
        target = room.beings.locate_first victim
        being.puts t('err_nobody') and next if target.nil?

        case action
        when 'join' then target.group.request_join being
        when 'withdraw' then target.group.request_withdraw being
        when 'accept' then being.group.request_accept target
        when 'deny' then being.group.request_deny target
        end
      else
        being.puts t('pending'), being.pending_group.leader unless being.pending_group.nil?

        being.puts t('members'), leader
        group.members.each do |member|
          being.puts t('member'), member,
                     member.health.current,
                     member.health.max,
                     member.mana.current,
                     member.mana.max,
                     member.stamina.current,
                     member.tnl
        end

        if being == leader && !group.pending.empty?
          being.puts t('pending_members')
          group.pending.each do |pend|
            being.puts t('member'), pend,
                       pend.health.current,
                       pend.health.max,
                       pend.mana.current,
                       pend.mana.max,
                       pend.stamina.current,
                       pend.tnl
          end
        end
      end
    end
  end

  ###############################################################################
  define :help do
    usage :command
    exec do
      if command
        cmd = ::Command.configuration.command_registry.find do |c|
          c.get_name == command.name
        end
        being.puts t('err_not_found'), command.name and next if cmd.nil?

        name  = cmd.get_name
        brief = cmd.get_brief
        usage = Util::Helper.print_usages_for cmd

        desc = if $help[:command].key?(cmd.get_name)
                 $help[:command][cmd.get_name]
               else
                 t('err_no_desc')
               end

        being.puts $templates.help, name, brief, usage, desc
      end
    end
  end

  ###############################################################################
  define :inventory do
    shortcut 'i'
    usage
    exec do
      being.puts t('title'), being.items.display_contents
    end
  end

  ###############################################################################
  define :kill do
    shortcut 'k'
    usage({ victim: :id })
    exec do
      targets = room.beings.locate victim
      being.puts t('err_noone') and next if targets.empty?
      being.puts t('err_too_many') and next if targets.length > 1

      target = targets.first

      being.puts t('title'), target
      Combat.embattle being, target
    end
  end

  ###############################################################################
  define :learn do
    usage({ ability: :string })
    exec do
      err_str = being.learn ability
      being.puts err_str and next if err_str
    end
  end

  ###############################################################################
  define :look do
    shortcut 'l'
    usage
    exec do
      exits = Realm::Exit::DIRECTIONS
              .keys
              .map { |dir| I18n.t("movement.exit.#{dir}") if being.room.send dir }
              .compact
              .join ' '

      being.puts t('title'), room, room
      being.puts t('exit_list'), exits

      being.other_beings.each { |b| being.puts t('here'), b }

      next if room.items.empty?

      being.puts t('item_list'), room.items.display_contents
    end
  end

  ###############################################################################
  # TODO implement "exploration" where players don't see Rooms mapped until they
  # have visited them. Could link up to achievements, exp bonuses, show up on
  # the summary page, etc.
  define :map do
    usage
    exec do
      zone   = being.room.zone
      layout = zone.layout
      buffer = ''

      being.puts t('title'), zone

      layout.reverse.each_with_index do |row, _row_index|
        row.each_with_index do |point, _col_index|
          buffer << case point
                    when Realm::Room
                      room == point ? '{C@{x' : 'x'
                    when Realm::Exit
                      point.north.nil? ? '-' : '|'
                    else
                      ' '
                    end
        end
        buffer << "\n"
      end

      being.puts buffer
    end
  end

  ###############################################################################
  define :practice do
    usage({ ability: :string })
    exec do
      err_str = being.practice ability
      being.puts err_str and next if err_str
    end
  end

  ###############################################################################
  define :quit do
    usage
    exec do
      being.cmd 'save'
      being.puts t('actor')
      being.user.client.should_terminate = true
    end
  end

  ###############################################################################
  define :remove do
    usage :target
    exec do
      targets = being.anatomy.locate target
      being.puts t('err_not_found') and next if targets.empty?

      targets.each do |item|
        message = being.anatomy.remove item, being
        being.puts(*message)
      end
    end
  end

  ###############################################################################
  define :save do
    usage
    exec do
      being.print t('start')
      Logistics::Scribe.save being
      being.puts t('success')
    end
  end

  ###############################################################################
  define :say do
    usage :rest
    exec do
      being.speak rest
    end
  end

  ###############################################################################
  define :shortcuts do
    usage
    exec do
      commands = []
      ::Command.configuration.command_sets.values.each { |set| commands << set.to_a }

      list = commands.flatten
                     .reject { |c| c.get_shortcut.nil? }
                     .select { |c| being.level >= (c.level || 0) }
                     .sort_by(&:get_shortcut)
                     .map { |c| '  %s - %s' % [c.get_shortcut, c.get_name] }

      being.puts t('title')
      being.puts list.join "\n"
    end
  end

  ###############################################################################
  define :skills do
    usage
    exec do
      being.puts t('title')
      being.puts_abilities_list :skill
    end
  end

  ###############################################################################
  define :spells do
    usage
    exec do
      being.puts t('title')
      being.puts_abilities_list :spell
    end
  end

  ###############################################################################
  define :summary do
    usage
    exec do
      being.puts t('title'), being.name, being.level_label
      being.puts being.summary_plate
    end
  end

  ###############################################################################
  define :wear do
    usage :target
    exec do
      targets = being.items.locate target
      being.puts t('err_not_found') and next if targets.empty?

      targets.each do |item|
        message = being.anatomy.wear item, being
        being.puts(*message)
      end
    end
  end

  ###############################################################################
  define :who do
    usage
    exec do
      being.puts t('title')
      $world.avatars.values.each do |a|
        being.puts '  ' + a.name
      end
    end
  end

  ###############################################################################
  define :whoami do
    usage
    exec do
      being.puts t('actor'), being
    end
  end

  ###############################################################################
  define :zones do
    usage
    exec do
      being.puts t('title')
      $world.zones.values.sort_by(&:rid).each do |zone|
        next if zone.rid == 1 && !being.demigod?

        being.puts t('line'), zone, zone, zone.rooms.length, zone.items.length, zone.npcs.length
      end
    end
  end
end
