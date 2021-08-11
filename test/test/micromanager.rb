require 'fileutils'

module Test
  class Micromanager
    def initialize(test_node)
      @node = test_node
      @test = @node.content
      @id   = @test.get_id

      go
    end

    def ancestral_path(n)
      if n.parent.name == :root
        n.name
      else
        "#{ancestral_path n.parent}..#{n.name}"
      end
    end

    def state_path
      @state_path ||= $ROOT_DIR.join('test', 'state', ancestral_path(@node))
    end

    def data_path
      @data_path ||= state_path.join('data')
    end

    def log_path
      @log_path ||= state_path.join('server-log.txt')
    end

    def go
      # Start the server thread.
      server_thread = start_server_thread

      start_test

      # Need to stop the server process before joining the thread.
      `kill -s USR1 #{IO.read(data_path.join('pidfile'))}`
      server_thread.join
    end

    def start_server_thread
      # If this is an independent test, create the data directory.
      if @test.get_parent_id.nil?
        FileUtils.mkdir_p data_path
      # Otherwise, copy the parent data to start with.
      else
        parent_data = $ROOT_DIR.join 'test', 'state', ancestral_path(@node.parent)
        FileUtils.cp_r parent_data, state_path
        # Truncate any logs.
        FileUtils.rm_rf Dir.glob(state_path.to_s + '/*-log.txt')
      end

      th = Thread.new do
        # Boot the server:
        #   * on an ephemeral port
        #   * with no loop throttling
        #   * with its own data directory (pursuant to above condition)
        #   * with file logging enabled
        #   * with maximum verbosity
        `#{$SYMPHONY} -p 0 -i 0 -d #{data_path} -o #{log_path} -l trace`
      end

      # Busywait for the server to boot.
      booted = false
      until booted
        sleep 0.01
        next unless File.exist? log_path

        booted = !/listening/.match(File.read(log_path)).nil?
      end
      th
    end

    def start_test
      # Get the ephemeral port the server is listening on from the log.
      matches = /listening on \d+\.\d+\.\d+\.\d+:(\d+)/.match(IO.read(log_path))
      port = matches[1]

      context = OpenStruct.new
      context.port = port
      context.state_path = state_path

      # For each 'with' account (e.g. "with :larry"), define a variable to
      # access the Client (e.g. "larry_client").
      @test.accounts.each do |acct|
        context[(acct + '_client').to_sym] = Client.new(port, state_path, acct)
      end

      @test.conduct_test context
    end
  end
end
