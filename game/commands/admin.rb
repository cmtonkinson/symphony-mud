require 'resolv'

module Command
  ###############################################################################
  define :at do
    level Realm::Being::DEMIGOD
    usage({ room_rid: :int }, :rest)
    exec do
      target = $world.find_room room_rid
      being.puts t 'err_nowhere' and next if target.nil?
      being.puts t 'err_here' and next if room == target

      current_room = room
      being.room = target
      begin
        being.cmd rest
      ensure
        being.room = current_room
      end
    end
  end

  ###############################################################################
  define :connections do
    level Realm::Being::GOD
    usage
    exec do
      dns_width = 35
      format = "%-15s{C%-6s{Y%-23s{W%-#{dns_width}s"
      being.puts format % %w[Name Sock IP:Port DNS]
      being.puts '-------------------------------------------------------------------------------'
      $universe.clients.each do |client|
        sock_domain, remote_port, remote_hostname, remote_ip = client.socket.peeraddr
        dns_name = Resolv.getname remote_ip
        being.puts format % [
          client.user.avatar.name,
          client.get_fd,
          "#{remote_ip}:#{remote_port}",
          Util::Linguist.truncate_middle(dns_name, dns_width)
        ]
      end
    end
  end

  ###############################################################################
  define :dashboard do
    level Realm::Being::CREATOR
    usage
    exec do
      # Job Performance
      job_stats = $world.schedule_stats 0
      run       = $world.job_times
      being.puts '{Y%d{x jobs in the queue running {Y%d{x at a time.' % [job_stats[:length], job_stats[:batch_size]]
      being.puts 'Job wall-time analysis (units in microseconds):'
      being.puts '  jobs run so far: {Y%d{x' % run.number
      being.puts '  mean: {Y%d{x' % run.mean
      being.puts '  median: {Y%d{x' % run.median
      being.puts '  stddev: {Y%.2f{x' % run.standard_deviation
      being.puts '  min: {Y%d{x' % run.min
      being.puts '  max: {Y%d{x' % run.max

      # Registry
      report = Util::Registry.report
      being.puts "\nThere are {C%d{x Objekts in the global registry at the moment." % report.size
      report[:buckets].each do |name, details|
        being.puts "  {c%6d{x %s\n" % [details[:size], name]
      end
    end
  end

  ###############################################################################
  define :destroy do
    level Realm::Being::DEMIGOD
    usage('item', target: :id)
    usage('npc', target: :id)
    exec do
      if item == 'item'
        targets = being.items.locate target
        being.puts t('err_no_item') and next if targets.empty?
        targets.each do |item|
          item.run_hook :enervate
          room.puts t('item'), item
          Symphony.logger.debug '%s destroyed %s' % [being.fqid_str, item.fqid_str]
        end
      elsif npc == 'npc'
        targets = room.beings.locate target
        being.puts t('err_no_npc') and next if targets.empty?
        targets.each do |victim|
          being.puts t('err_not_npc') and next unless victim.is_a? Realm::NPC
          victim.run_hook :enervate
          room.puts t('npc'), victim
          Symphony.logger.debug '%s destroyed %s' % [being.fqid_str, victim.fqid_str]
        end
       end
    end
  end

  ###############################################################################
  define :dig do
    level Realm::Being::DEMIGOD
    usage direction: :string
    exec do
      being.puts t 'err_no_way' and next unless Realm::Exit::DIRECTIONS.key?(direction)
      being.puts t 'err_already' and next unless room.send(direction).nil?

      Logistics::Builder.create_linked_room room, direction

      being.puts t('actor'), direction
      room.puts t('other'), being, direction, exclude: being

      being.cmd direction
    end
  end

  ###############################################################################
  define :disconnect do
    level Realm::Being::GOD
    usage({ victim: :string })
    exec do
      target = $world.find_avatar victim
      being.puts t 'err_nobody' and next if target.nil?
      being.puts t 'err_self' and next if target == being
      being.puts t('err_seniority'), target and next unless being.has_seniority? target

      being.puts t('actor'), target
      target.puts t('victim'), being
      target.cmd 'quit'
    end
  end

  ###############################################################################
  define :force do
    level Realm::Being::GOD
    usage({ victim: :string }, :rest)
    exec do
      target = room.find_being victim
      being.puts t 'err_nobody' and next if target.nil?
      being.puts t 'err_self' and next if target == being
      being.puts t('err_seniority'), target and next unless being.has_seniority? target

      being.puts t('actor'), target
      target.puts t('victim'), being
      target.cmd rest
    end
  end

  ###############################################################################
  define :goto do
    level Realm::Being::DEMIGOD
    usage({ room_id: :int })
    exec do
      target = $world.find_room room_id
      being.puts t 'err_nowhere' and next if target.nil?
      being.puts t 'err_there' and next if room == target

      being.puts t('actor'), target
      room.puts t('leave'), being, exclude: being
      target.puts t('arrive'), being
      being.transport target
    end
  end

  ###############################################################################
  define :heal do
    level Realm::Being::GOD
    usage({ victim: :id })
    exec do
      targets = room.beings.locate victim
      being.puts t 'err_nobody' and next if targets.empty?

      targets.each do |target|
        being.puts t('actor'), target
        target.puts t('victim'), being
        target.heal
      end
    end
  end

  ###############################################################################
  define :identify do
    level Realm::Being::DEMIGOD
    usage({ item: :string })
    exec do
      target = being.items.locate_first item
      being.puts t 'err_not_found' and next if target.nil?

      being.puts target.summary
    end
  end

  ###############################################################################
  define :jobs do
    level Realm::Being::CREATOR
    usage
    usage({ jobs: :int })
    exec do
      num_jobs = jobs.nil? ? 10 : jobs
      stats    = $world.schedule_stats num_jobs

      being.puts t('queue_length') % stats[:length]
      being.puts t('batch_size') % stats[:batch_size]
      being.puts t('next_jobs') % num_jobs
      being.puts stats[:up_next].map(&:to_s).join "\n"
    end
  end

  ###############################################################################
  define :load do
    level Realm::Being::DEMIGOD
    usage('item', { rid: :int })
    usage('npc', { rid: :int })
    exec do
      if item == 'item'
        model = being.room.zone.find_by_rid Realm::Item, rid
        being.puts t 'err_nothing' and next if model.nil?

        copy = model.clone
        copy.add.to being
        copy.run_hook :vivify
        being.puts t('actor'), copy
        Symphony.logger.debug '%s loaded %s as a clone of %s' % [being.fqid_str, copy.fqid_str, model.fqid_str]
      elsif npc == 'npc'
        model = being.room.zone.find_by_rid Realm::NPC, rid
        being.puts t 'err_nothing' and next if model.nil?

        copy = model.clone
        copy.add.to room.beings
        copy.room = room
        copy.run_hook :vivify
        being.puts t('actor'), copy
        Symphony.logger.debug '%s loaded %s as a clone of %s' % [being.fqid_str, copy.fqid_str, model.fqid_str]
       end
    end
  end

  ###############################################################################
  define :log_level do
    level Realm::Being::CREATOR
    usage({ new_level: %w(trace debug info warn error fatal) })
    exec do
      Symphony.logger.level = Logger.const_get(new_level.upcase)
      being.puts t('changed'), Symphony.logger.level
      Symphony.logger.unknown "Log level changed dynamically to #{new_level.upcase} by #{being.fqid_str}"
    end
  end

  ###############################################################################
  define :peace do
    level Realm::Being::GOD
    usage
    exec do
      room.beings.values.each { |b| b.group.remove_all_opponents }
      room.puts t('room')
    end
  end

  ###############################################################################
  define :promote do
    level Realm::Being::GOD
    usage({ target: :id }, { to_level: :int })
    exec do
      victim = room.beings.locate_first target
      being.puts t('err_not_found') and next if victim.nil?
      being.puts t('err_no_permission') and next if victim == being
      being.puts t('err_already_there') and next if victim.level >= to_level
      being.puts t('err_no_permission') and next unless being.has_seniority?(victim)
      being.puts t('err_no_permission') and next if to_level > being.level - Realm::Being::IMMORTAL_SENIORITY

      being.puts t('actor'), victim
      victim.puts t('victim'), being

      # When the victim is an Avatar and the target level is above HERO, we need
      # to account for the fact that their TNL will stay at 0. So we award normal
      # experience up through HERO, and then we have to directly trigger the
      # level gains.
      victim.award_experience victim.tnl while victim.tnl > 0 && victim.level < to_level
      victim.gain_level while victim.level < to_level
    end
  end

  ###############################################################################
  define :pry do
    level Realm::Being::CREATOR
    usage
    exec do
      binding.pry
    end
  end

  ###############################################################################
  define :reboot do
    level Realm::Being::CREATOR
    usage
    exec do
      being.puts t('actor')
      $universe.should_reboot = true
    end
  end

  ###############################################################################
  define :purge do
    level Realm::Being::GOD
    usage
    exec do
      room.puts t('room')
      room.items.values.each { |i| i.run_hook :enervate }
      room.beings.values.each { |b| b.run_hook :enervate if b.is_a? Realm::NPC }
    end
  end

  ###############################################################################
  define :repeat do
    level Realm::Being::CREATOR
    usage({ times: :int }, :rest)
    exec do
      min = 1
      max = 1000
      being.puts t('err_range') % [min, max] and next if times < min || times > max

      (0..times).each { being.cmd rest }
    end
  end

  ###############################################################################
  define :reset do
    level Realm::Being::GOD
    usage
    usage 'all'
    exec do
      if all == 'all'
        $world.zones.values.each &:reset
        being.puts t('all')
      else
        room.zone.reset
        being.puts t('current')
      end
    end
  end

  ###############################################################################
  define :rooms do
    level Realm::Being::DEMIGOD
    usage
    usage({ id: :int })
    exec do
      zone = id.nil? ? being.room.zone : $world.zones.values.find { |z| z.rid == id }
      being.puts t 'err_nowhere' and next if zone.nil?

      being.puts t('title'), zone
      zone.rooms.each do |id, room|
        being.puts t('line'), room, room
      end
    end
  end

  ###############################################################################
  define :shutdown do
    level Realm::Being::CREATOR
    usage
    exec do
      being.puts t('actor')
      $universe.should_stop = true
    end
  end

  ###############################################################################
  define :summon do
    level Realm::Being::GOD
    usage({ victim: :string })
    exec do
      target = $world.find_avatar victim
      being.puts t('err_self') and next if target == being
      being.puts t('err_nobody') and next if target.nil?
      being.puts t('err_here'), target and next if target.room == room
      being.puts t('err_seniority'), target and next unless being.has_seniority? target

      being.puts t('actor'), target
      target.puts t('victim'), being, being
      target.room.puts t('leave'), target, exclude: target
      room.puts t('arrive'), target
      target.transport room
    end
  end

  ###############################################################################
  # NOTE: This method is designed to be used for R&D. See the O'RLY Essentials
  #       book, "Changing Stuff and Seeing What Happens" (aka "The Kitten Book")
  define :test do
    level Realm::Being::CREATOR
    usage
    exec do
    end
  end

  ###############################################################################
  define :tutor do
    level Realm::Being::CREATOR
    usage({ target: :id })
    exec do
      victim = room.beings.locate_first target
      being.puts t('err_nobody') and next if victim.nil?

      victim.abilities.each(&:master)
      victim.puts t('victim'), being
      being.puts t('actor'), victim
    end
  end

  ###############################################################################
  define :worldsave do
    level Realm::Being::CREATOR
    usage
    exec do
      being.print t('start')
      $world.save
      being.puts t('success')
    end
  end
end
