module Network

  class Client
    attr_accessor :socket, :terminate

    def initialize(tcp_socket)
      @socket = tcp_socket
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
      self.socket.close
    end

  end

end
