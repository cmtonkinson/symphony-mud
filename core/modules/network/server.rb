module Network

  class Server < TCPServer

    def initialize
      config = Network.configuration
      STDOUT.puts "#{self.class.name} listening on #{config.listen_ip}:#{config.listen_port}"
      super config.listen_ip, config.listen_port
    end

    def accept
      begin
        connection = Network::Connection.new accept_nonblock
      rescue Errno::EWOULDBLOCK, Errno::EINTR
        connection = nil
      end
      connection
    end

  end

end
