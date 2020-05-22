require 'fileutils'
require 'net-telnet'

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
      puts "########################## Testing #{@test.get_id}"

      # Start the server thread.
      server_thread = start_server_thread
 
      # Start the test thread and wait for it to finish running.
      # It will join once it's work is complete.
      test_thread = start_test_client_thread
      test_thread.join

      # Need to stop the server process before joining the thread.
      `kill -s USR1 #{IO.read(data_path.join('pidfile'))}`
      server_thread.join

      puts "########################## done"
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

      # Give the server a moment to boot.
      sleep 1
      return th
    end

    ###########################################################################
    def start_test_client_thread
      # Get the ephemeral port the server is listening on from the log.
      matches = /listening on 0\.0\.0\.0:(\d+)/.match(File.read log_path)
      port = matches[1]

      client_thread = Thread.new {
#        @test.conduct_test(port, state_path)
        client = Client.new(port, state_path, 'foo')
        client.command 'foo'
        client.command 'foo'
        client.command 'foo'
        client.command 'foo'
        client.command 'quit'
        client.close
      }
    end

  end
end

