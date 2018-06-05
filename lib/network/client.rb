module Network
  class Client
    attr_accessor :socket, :should_terminate, :user

    def initialize(tcp_socket)
      @should_terminate = false
      @socket           = tcp_socket
      @user             = nil

      Symphony.logger.info "New Client on socket #{get_fd}"
    end

    def get_fd
      socket.to_i
    end

    def print(message)
      _send message
    end

    def puts(message)
      _send "#{message}{x\n"
    end

    def recv
      begin
        input = socket.recv_nonblock Network.configuration.recv_maxlen
        return input.strip
      rescue Errno::EWOULDBLOCK, Errno::EINTR
        return nil
      end
    end

    def close
      quote = $universe.quotes.sample
      self.puts "\n\n"
      self.puts quote['body']
      self.puts "  -#{quote["author"]}\n\n"
      Symphony.logger.info "Closing Client on socket #{get_fd}"
      self.socket.close
    end

    private

    def _send(message)
      socket.send ::Base::Terminal.colorize(message), 0
    end
  end
end
