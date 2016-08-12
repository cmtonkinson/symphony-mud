require "resolv"

module Command

###############################################################################
define :connections do
###############################################################################
  level Realm::Being::GOD
  usage
  exec do
    dns_width = 35
    format = "%-15s{C%-6s{Y%-23s{W%-#{dns_width}s"
    being.puts format % %w(Name Sock IP:Port DNS)
    being.puts "-------------------------------------------------------------------------------"
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
define :disconnect do
###############################################################################
  level Realm::Being::GOD
  usage({victim: :string})
  exec do
    target = $world.find_avatar victim
    being.puts t "err_nobody" and next if target.nil?
    being.puts t "err_self" and next if target == being

    being.puts t("actor"), target
    target.puts t("victim"), being
    target.cmd "quit"
  end
end

###############################################################################
define :goto do
###############################################################################
  level Realm::Being::DEMIGOD
  usage({room_id: :int})
  exec do
    target = $world.find_room room_id
    being.puts t "err_nowhere" and next if target.nil?
    being.puts t "err_there" and next if room == target

    being.puts t("actor"), target
    room.puts t("leave"), being, exclude: being
    target.puts t("arrive"), being
    being.transport target
  end
end

###############################################################################
define :jobs do
###############################################################################
  level Realm::Being::CREATOR
  usage
  exec do
    num_jobs = 10
    stats    = $world.schedule_stats num_jobs

    being.puts t("queue_length") % stats[:length]
    being.puts t("batch_size") % stats[:batch_size]
    being.puts t("next_jobs") % num_jobs
    being.puts stats[:up_next].map(&:to_s).join "\n"
  end
end

###############################################################################
define :reboot do
###############################################################################
  level Realm::Being::CREATOR
  usage
  exec do
    being.puts t("actor")
    $universe.should_reboot = true
  end
end

###############################################################################
define :rooms do
###############################################################################
  usage
  usage({id: :int})
  exec do
    zone = id.nil? ? being.room.zone : $world.zones.values.find { |z| z.rid == id }
    being.puts t "err_nowhere" and next if zone.nil?

    being.puts t("title"), zone
    zone.rooms.each do |id,room|
      being.puts t("line"), room, room
    end
  end
end

###############################################################################
define :shutdown do
###############################################################################
  level Realm::Being::CREATOR
  usage
  exec do
    being.puts t("actor")
    $universe.should_stop = true
  end
end

###############################################################################
define :summon do
###############################################################################
  level Realm::Being::GOD
  usage({victim: :string})
  exec do
    target = $world.find_avatar victim
    being.puts t("err_self") and next if target == being
    being.puts t("err_nobody") and next if target.nil?
    being.puts t("err_here"), target and next if target.room == room

    being.puts t("actor"), target
    target.puts t("victim"), being, being
    target.room.puts t("leave"), target, exclude: target
    room.puts t("arrive"), target
    target.transport room
  end
end

###############################################################################
define :worldsave do
###############################################################################
  level Realm::Being::CREATOR
  usage
  exec do
    being.print t("start")
    $world.save
    being.puts t("success")
  end
end

end
