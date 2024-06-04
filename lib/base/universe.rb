require 'yaml'

module Base
  class Universe
    attr_accessor :clients, :server, :should_stop, :quotes, :should_reboot,
                  :should_save

    def initialize
      @clients     = []
      @quotes      = []
      @should_save = false
      @should_stop = false
    end

    def do_server
      setup_server
      Symphony.logger.unknown "Server listening on #{@server.ip_port_s}"
      until should_stop
        welcome server.accept
        $world.handle_jobs
        handle_input
        handle_output
        handle_disconnections
        begin_reboot if should_reboot
        Util::Registry.cleanup
        sleep(Base.configuration.sleep_interval / 1000.0)
      end
      $world.save if should_save
      Symphony.logger.unknown 'Server shutting down.'
      File.delete Symphony.pidfile
    end

    def setup_server
      if ENV['SYMPHONY_REBOOT'] == 'true'
        recover_from_reboot
      else
        @server = Network::Server.new
      end
      @quotes = YAML.load_file(Symphony.root.join('data', 'quotes.yml'))
    end

    def welcome(client)
      return if client.nil?

      user = User.new
      client.puts $templates.welcome_banner
      user.bind_client client
      clients << client
    end

    def handle_input
      clients.each do |client|
        input = client.recv
        next if input.nil?

        # Bang is a shortcut for "run the last input again". We keep input
        # history in a buffer so either we need to examine the top of that
        # buffer or add to it.
        if input == '!'
          input = client.last_input
        else
          client.last_input = input
        end

        begin
          client.user.handle input
        rescue StandardError => e
          Symphony.logger.fatal e
          Symphony.logger.error 'Forcing termination of client on socket %d' % [client.fd]
          client.should_terminate = true
        end
      end
    end

    def handle_output
      clients.each do |client|
        user   = client.user
        avatar = user.avatar
        target = nil

        if avatar
          target = avatar.interactive || client
          target.print avatar.output! if avatar.output? || avatar.test_and_clear_forced_output
        end
      end
    end

    def handle_disconnections
      clients.select(&:should_terminate).each do |c|
        if (avatar = c.user.avatar)
          avatar.run_hook :before_disconnect
          avatar.dispose
        end
        c.close
        clients.delete c
      end
    end

    def broadcast(message)
      formatted = message.prepend I18n.t('universe.broadcast')
      Symphony.logger.debug formatted
      @clients.each { |client| client.puts formatted }
    end

    def begin_reboot
      Symphony.logger.unknown 'Server beginning reboot'
      broadcast I18n.t('universe.reboot.begin')
      # Syntax check.
      Util::Linter.lint_project_source
      # Save socket -> avatar mapping.
      save_reboot_map
      # Begin anew.
      env, options = reboot_exec_details
      cli = Symphony.binary.to_s + $command_line_args
      [env, cli, options].each { |x| Symphony.logger.warn "  #{x.inspect}" }
      Process.exec env, cli, options
    rescue StandardError => e
      @should_reboot = false
      Symphony.logger.unknown 'Reboot failed'
      e.message.split("\n").each { |error_line| Symphony.logger.debug error_line }
      broadcast I18n.t('universe.reboot.fail')
    end

    def reboot_exec_details
      # We want to pass all open file descriptors to the new process.
      server_fd  = server.to_i
      client_fds = clients.map(&:fd)
      env = {
        'SYMPHONY_REBOOT' => 'true',
        'SYMPHONY_SERVER_FD' => server_fd.to_s,
        'SYMPHONY_CLIENT_FDS' => Marshal.dump(client_fds)
      }
      options = {
        in: :in,
        out: :out,
        err: :err,
        close_others: false
      }
      # Add the server socket to the options Hash.
      options[server_fd] = server_fd
      # Add all the client sockets to the options Hash.
      @clients.each { |c| options[c.fd] = c.fd }
      # Return both env and options hashes.
      [env, options]
    end

    def recover_from_reboot
      # Restart the server.
      @server = Network::Server.for_fd ENV['SYMPHONY_SERVER_FD'].to_i
      # Restore user connections.
      load_reboot_map
      # Clean up ENV.
      ENV.delete 'SYMPHONY_REBOOT'
      ENV.delete 'SYMPHONY_SERVER_FD'
      ENV.delete 'SYMPHONY_CLIENT_FDS'
      Symphony.logger.unknown 'Server recovered from reboot'
      broadcast I18n.t('universe.reboot.succeed')

      # Force everyone to "look" around.
      @clients.map(&:user).map(&:avatar).each { |a| a.cmd 'look' }
    end

    # Store to disk a mapping of client socket descriptors to Avatar ids.
    def save_reboot_map
      recovery_map = @clients.map { |c| [c.fd, c.user.avatar.id] }.to_h
      File.write Base.configuration.recovery_file, YAML.dump(recovery_map)
    end

    # Reconnect all Avatars back to their corresponding descriptors.
    def load_reboot_map
      # Read the recovery Hash.
      recovery_map = YAML.load_file(Base.configuration.recovery_file)

      # rubocop:disable Security/MarshalLoad
      Marshal.load(ENV.fetch('SYMPHONY_CLIENT_FDS', nil)).each do |fd|
        client = Network::Client.new TCPSocket.for_fd fd.to_i
        user   = User.new
        user.bind_client client, false
        user.bind_avatar Logistics::Scribe.load Realm::Avatar, recovery_map[fd]
        user.avatar.run_hook :vivify
        @clients << client
      end
      # rubocop:enable Security/MarshalLoad
    end

    # Laughably simplistic REPL, but gets the job done for now.
    def do_console
      context = binding
      loop do
        print '> '
        input = gets.chomp!
        begin
          result = context.eval input
        rescue StandardError => e
          puts "StandardError: #{e}"
        end
        puts "=> #{result}"
      end
    end

    def do_script
      context = binding
      content = File.read Symphony.configuration.script
      context.eval content
    end
  end
end
