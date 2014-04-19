module Network

  class Server < TCPServer

    def initialize
      config = Network.configuration
      STDOUT.puts "#{self.class.name} listening on #{config.listen_ip}:#{config.listen_port}"
      super config.listen_ip, config.listen_port
    end

    def accept
      client = nil
      begin
        client = Network::Client.new accept_nonblock
      rescue Errno::EWOULDBLOCK, Errno::EINTR
      end
      client
    end

  end

end
