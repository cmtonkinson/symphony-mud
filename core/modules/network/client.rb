module Network

  class Client
    attr_accessor :socket

    def initialize(tcp_socket)
      @socket = tcp_socket
    end

    def get_fd
      self.socket.to_i
    end

    def puts(*args)
      self.socket.puts args
    end

    def close
      self.socket.close
    end

  end

end
