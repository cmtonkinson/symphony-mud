module Network

  class Connection

    def initialize(tcp_socket)
      @socket = tcp_socket
    end

    def get_fd
      @socket.to_i
    end

    def puts(*args)
      @socket.puts args
    end

    def close
      @socket.close
    end

  end

end
