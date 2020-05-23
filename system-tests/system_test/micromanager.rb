require 'fileutils'

module SystemTest
  class Micromanager

    ###########################################################################
    def initialize(test_node)
      @node = test_node
      @test = @node.content
      @id   = @test.get_id

      go
    end

    ###########################################################################
    def state_path
      @state_path ||= $ROOT_DIR.join('system-tests', 'state', @id)
    end

    def data_path
      @data_path ||= state_path.join('data')
    end

    def log_path
      @log_path ||= state_path.join('server-log.txt')
    end

    ###########################################################################
    def go
      # Start the server thread.
      server_thread = start_server_thread
 
      start_test

      # Need to stop the server process before joining the thread.
      `kill -s USR1 #{IO.read(data_path.join('pidfile'))}`
      server_thread.join
    end

    ###########################################################################
    def start_server_thread
      # Ensure the data directory is clean.
      FileUtils.rm_rf state_path

      # If this is an independent test, create the data directory.
      if @test.get_parent_id.nil?
        FileUtils.mkdir_p data_path
      # Otherwise, copy the parent data to start with.
      else
        parent_data = $ROOT_DIR.join 'system-tests', 'state', @test.get_parent_id
        FileUtils.cp_r parent_data, state_path
        # Truncate any logs.
        FileUtils.rm_rf Dir.glob(state_path.to_s + '/*-log.txt') 
      end

      th = Thread.new {
        # Boot the server:
        #   * on an ephemeral port
        #   * with no loop throttling
        #   * with its own data directory (pursuant to above condition)
        #   * with file logging enabled
        #   * with maximum verbosity
        `#{$SYMPHONY} -p 0 -i 0 -d #{data_path} -o #{log_path} -l trace`
      }

      # Busywait for the server to boot.
      booted = false
      until booted do
        sleep 0.01
        next unless File.exist? log_path
        booted = !/listening/.match(File.read log_path).nil?
      end
      return th
    end

    ###########################################################################
    def start_test
      # Get the ephemeral port the server is listening on from the log.
      matches = /listening on \d+\.\d+\.\d+\.\d+:(\d+)/.match(IO.read log_path)
      port = matches[1]

      context = OpenStruct.new
      context.port = port
      context.state_path = state_path

      acct = @test.get_account_name
      if acct
        client = Client.new port, state_path, acct
        # Reference the connection by either "client" or the account name.
        context.client = client
        context[acct.to_sym] = client
      end

      @test.conduct_test context
    end

  end
end

