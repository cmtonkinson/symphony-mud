require "resolv"

module Command

###############################################################################
define :connections do
###############################################################################
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
  usage({victim: :string})
  brief "Disconnect the victim from the server."
  exec do
    target = $world.find_avatar victim
    being.puts "They're not in this realm." and next if target.nil?
    being.puts "You can't disconnect yourself." and next if target == being

    target.puts "You have been unceremoniously disconnected by $being.", being
    target.cmd "quit"
  end
end

###############################################################################
define :goto do
###############################################################################
  usage({room_id: :int})
  brief "Transport yourself to the given room."
  exec do
    target = $world.find_room room_id
    being.puts "Room not found." and next if target.nil?
    being.puts "You're already there!" and next if room == target

    being.puts I18n.t("goto.you"), target
    room.puts I18n.t("goto.leave"), being, exclude: being
    target.puts I18n.t("goto.arrive"), being
    being.transport target
  end
end

###############################################################################
define :jobs do
###############################################################################
  usage
  brief "List scheduler statistics."
  exec do
    jobs_to_inspect = 10

    stats = $world.schedule_stats jobs_to_inspect
    being.puts "There are {Y%u{x jobs in the queue." % stats[:length]
    being.puts "The schedule runs {Y%u{x jobs at a time." % stats[:batch_size]

    being.puts "\nThe next #{jobs_to_inspect} jobs:"
    being.puts stats[:up_next].map(&:to_s).join "\n"
  end
end

###############################################################################
define :reboot do
###############################################################################
  usage
  exec do
    being.puts "Hold on to your butts..."
    $universe.should_reboot = true
  end
end

###############################################################################
define :shutdown do
###############################################################################
  usage
  exec do
    being.puts "Shutting down the server."
    $universe.should_stop = true
  end
end

###############################################################################
define :summon do
###############################################################################
  usage({victim: :string})
  brief "Yank a person from wherever they are to appear before you."
  exec do
    target = $world.find_avatar victim
    being.puts "You can't summon yourself." and next if victim == being
    being.puts "They're not in this realm." and next if target.nil?
    being.puts "But they're already here!" and next if target.room == room

    being.puts I18n.t("summon.actor"), target
    target.puts I18n.t("summon.victim"), being, being
    target.room.puts I18n.t("summon.leave"), target, exclude: target
    room.puts I18n.t("summon.arrive"), target
    target.transport room
  end
end

###############################################################################
define :worldsave do
###############################################################################
  usage
  exec do
    being.print "Saving the World..."
    $world.save
    being.puts " success!"
  end
end

end
