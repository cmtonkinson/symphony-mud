require 'yaml'

module Base

  class Universe
    attr_accessor :clients, :server, :should_stop, :quotes, :should_reboot

    ###########################################################################
    def initialize
      @clients     = []
      @quotes      = []
      @should_stop = false
    end

    ###########################################################################
    def do_server
      setup_server

      Symphony.logger.info "Server listening on #{Network.configuration.listen_ip}:#{Network.configuration.listen_port}"
      until should_stop do
        welcome server.accept
        handle_input
        handle_disconnections
        begin_reboot if should_reboot
        sleep Base.configuration.sleep_interval
      end
      Symphony.logger.info "Server shutting down."
    end

    ###########################################################################
    def setup_server
      if ENV["SYMPHONY_REBOOT"] == "true"
        recover_from_reboot
      else
        @server = Network::Server.new
      end
      @quotes = YAML.load IO.read Symphony.root.join "data", "quotes.yml"
    end

    ###########################################################################
    def welcome(client)
      return if client.nil?
      user = User.new
      client.puts $templates.welcome_banner
      user.bind_client client
      clients << client
    end

    ###########################################################################
    def handle_input
      clients.each do |client|
        input = client.recv
        next if input.nil?
        client.user.operators.peek.handle input
      end
    end

    ###########################################################################
    def handle_disconnections
      clients.select { |c| c.should_terminate }.each do |c|
        c.user.avatar.run_hook :before_disconnect if c.user.avatar
        c.close
        clients.delete c
      end
    end

    ###########################################################################
    def broadcast(message)
      formatted = message.prepend I18n.t("universe.broadcast")
      Symphony.logger.debug formatted
      @clients.each { |client| client.puts formatted }
    end

    ###########################################################################
    def begin_reboot
      begin
        Symphony.logger.warning "Server beginning reboot"
        broadcast I18n.t("universe.reboot.begin")
        server_fd  = server.to_i
        client_fds = clients.map { |c| c.get_fd }
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
        # Store to disk a mapping of client socket descriptors to Avatar ids.
        recovery_map = Hash[@clients.map { |c| [c.get_fd, c.user.avatar.id] }]
        File.write Base.configuration.recovery_file, YAML.dump(recovery_map)
        # Add the server socket to the options Hash.
        options[server_fd] = server_fd
        # Add all the client sockets to the options Hash.
        @clients.each { |c| options[c.get_fd] = c.get_fd }
        # Begin anew.
        Process.exec env, Symphony.binary.to_s, options
      rescue Exception => e
        @should_reboot = false
        Symphony.logger.error "Reboot failed: #{e}"
        broadcast I18n.t("universe.reboot.fail")
      end
    end

    ###########################################################################
    def recover_from_reboot
      # Read the recovery Hash.
      recovery_map = YAML.load File.read Base.configuration.recovery_file
      # Restart the server.
      @server = Network::Server.for_fd ENV["SYMPHONY_SERVER_FD"].to_i
      # Reconnect all the clients.
      Marshal.load(ENV["SYMPHONY_CLIENT_FDS"]).each do |fd|
        client = Network::Client.new TCPSocket.for_fd fd.to_i
        user = User.new
        user.bind_client client, false
        user.bind_avatar Realm::Scribe.load Realm::Avatar, recovery_map[fd]
        user.puts "Welcome back!"
        @clients << client
      end
      # Clean up ENV.
      ENV.delete "SYMPHONY_REBOOT"
      ENV.delete "SYMPHONY_SERVER_FD"
      ENV.delete "SYMPHONY_CLIENT_FDS"
      Symphony.logger.warning "Server recovered from reboot"
      broadcast I18n.t("universe.reboot.succeed")
    end

    ###########################################################################
    # Laughably simplistic REPL, but gets the job done for now.
    def do_console
      context = binding
      loop do
        print "> "
        input = gets.chomp!
        begin
          result = context.eval input
        rescue Exception => e
          puts "Exception: #{e}"
        end
        puts "=> #{result}"
      end
    end

    ###########################################################################
    def do_script
      context = binding
      content = File.read Symphony.configuration.script
      context.eval content
    end

  end

end
