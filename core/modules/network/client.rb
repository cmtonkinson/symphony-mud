module Network

  class Client
    attr_accessor :socket, :should_terminate, :user

    def initialize(tcp_socket)
      @should_terminate = false
      @socket           = tcp_socket
      @user             = nil
    end

    def get_fd
      self.socket.to_i
    end

    def send(message)
      self.socket.send message, 0
    end

    def puts(message)
      self.send "#{message}\n"
    end

    def recv
      begin
        input = self.socket.recv_nonblock(10)
        return input.strip
      rescue Errno::EWOULDBLOCK, Errno::EINTR
        return nil
      end
    end

    def close
      quote = $universe.quotes.sample
      self.puts "\n\n"
      self.puts quote["body"]
      self.puts "  -#{quote["author"]}\n\n"
      self.socket.close
    end

  end

end
