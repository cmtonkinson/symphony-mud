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
define :reboot do
###############################################################################
  usage
  exec do
    being.puts "Hold on to your butts... Copyover!!"
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

end
