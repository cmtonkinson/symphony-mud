require 'socket'

module Network
  class Server < TCPServer
    def initialize
      config = Network.configuration
      super(config.listen_ip, config.listen_port)
    end

    def accept
      client = nil
      begin
        client = Network::Client.new accept_nonblock
      rescue Errno::EWOULDBLOCK, Errno::EINTR
        # These are swallowed on purpose because normal process signals can
        # interrupt the accept syscall and we can't treat them as failures.
      end

      client
    end

    def address
      local_address.ip_address
    end

    def port
      local_address.ip_port
    end

    def ip_port_s
      "#{address}:#{port}"
    end
  end
end
