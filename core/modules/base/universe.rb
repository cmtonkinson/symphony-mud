module Base

  class Universe
    attr_accessor :clients, :server, :should_stop, :should_reboot

    def initialize
      @clients     = []
      @should_stop = false
      setup_server
    end

    def setup_server
      if ENV["SYMPHONY_REBOOT"] == "true"
        recover_from_reboot
      else
        @server = Network::Server.new
      end
    end

    def live
      until self.should_stop do
        welcome self.server.accept
        handle_input
        handle_disconnections
        begin_reboot if self.should_reboot
        sleep Base.configuration.sleep_interval
      end
    end

    def welcome(client)
      return if client.nil?
      client.puts "Welcome!"
      self.clients << client
    end

    def handle_input
      self.clients.each do |c|
        input = c.recv
        next if input.nil?
        operator = Command::Operator.new c, Command.configuration.command_sets[:global]
        operator.handle input
      end
    end

    def handle_disconnections
      self.clients.select { |c| c.terminate }.each do |c|
        c.close
        self.clients.delete c
      end
    end

    def begin_reboot
      server_fd  = self.server.to_i
      client_fds = self.clients.map { |c| c.get_fd }
      env = {
        "SYMPHONY_REBOOT"     => "true",
        "SYMPHONY_SERVER_FD"  => server_fd.to_s,
        "SYMPHONY_CLIENT_FDS" => Marshal.dump(client_fds),
      }
      options = {
        :in           => :in,
        :out          => :out,
        :err          => :err,
        :close_others => false,
      }
      # Add the server socket to the options Hash.
      options[server_fd] = server_fd
      # Add all the client sockets to the options Hash.
      @clients.each { |c| options[c.get_fd] = c.get_fd }
      # Begin anew.
      Process.exec env, Symphony.binary.to_s, options
    end

    def recover_from_reboot
      # Restart the server.
      @server = Network::Server.for_fd ENV["SYMPHONY_SERVER_FD"].to_i
      # Reconnect all the clients.
      Marshal.load(ENV["SYMPHONY_CLIENT_FDS"]).each do |fd|
        client = Network::Client.new TCPSocket.for_fd fd.to_i
        client.puts "Welcome back!"
        @clients << client
      end
      # Clean up ENV.
      ENV.delete "SYMPHONY_REBOOT"
      ENV.delete "SYMPHONY_SERVER_FD"
      ENV.delete "SYMPHONY_CLIENT_FDS"
      STDOUT.puts "Listening once more!"
    end

  end

end
