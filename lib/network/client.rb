module Network
  class Client
    attr_accessor :socket, :telnet, :should_terminate, :user, :last_input,
      :telnet_data

    def initialize(tcp_socket)
      @should_terminate = false
      @socket           = tcp_socket
      @user             = nil
      @last_input       = ''
      @telnet_data      = {}
      @telnet           = Telnet.new(self)
      Symphony.logger.info "New Client on socket #{fd}"

      @telnet.negotiate
    end

    def fd
      socket.to_i
    end

    def print(message)
      _send message
    end

    def puts(message)
      _send "#{message}{x\n"
    end

    def recv
      input = socket.recv_nonblock Network.configuration.recv_maxlen

      if input[0].unpack("C")[0] == Telnet::TNET_IAC
        # Telnet in-band option negotiation sets the first byte as IAC.
        @telnet.recv_command input
        input = nil
      else
        # Otherwise, this is normal user input.
        input.strip
      end
    rescue Errno::EWOULDBLOCK, Errno::EINTR
      nil
    rescue Errno::ECONNRESET => e
      Symphony.logger.fatal e
      Symphony.logger.error 'Forcing termination of client on socket %d' % [fd]
      @should_terminate = true
    end

    def close
      quote = $universe.quotes.sample
      self.puts "\n\n"
      self.puts quote['body']
      self.puts "  -#{quote['author']}\n\n"
      Symphony.logger.info "Closing Client on socket #{fd}"
      socket.close
    end

    private

    def _send(message)
      if user&.avatar&.pref_show_color?
        socket.send ::Base::Terminal.colorize(message), 0
      else
        socket.send ::Base::Terminal.uncolor(message), 0
      end
    rescue Errno::EPIPE => e
      Symphony.logger.fatal e
      Symphony.logger.error 'Forcing termination of client on socket %d' % [fd]
      @should_terminate = true
    end
  end
end
