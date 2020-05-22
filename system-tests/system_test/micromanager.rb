require 'fileutils'

module SystemTest
  class Micromanager

    ###########################################################################
    def initialize(test_node)
      @node = test_node
      @test = @node.content
      
      if @node.name != :root
        @id = @test.get_id
        go
      end
    end

    ###########################################################################
    def state_path
      @state_path ||= $ROOT_DIR.join('system-tests', 'state', @id)
    end

    def data_path
      @data_path ||= state_path.join('data')
    end

    def log_path
      @log_path ||= state_path.join('log.txt')
    end

    ###########################################################################
    def go
      puts "#############################################"
      puts "Testing #{@test.get_id}"

      server_thread = Thread.new {
        # Clear out data from the last run of this test.
        FileUtils.rm_rf state_path
        # Ensure the data directory exists.
        FileUtils.mkdir_p data_path
        # Boot the server.
        `#{$SYMPHONY} -p 0 -i 0 -d #{data_path} -o #{log_path} -l trace`
      }

      client_thread = Thread.new {
        # Wait for the server to boot.
        sleep 1

        # Get the ephemeral port the server is listening on from the log.
        matches = /listening on 0\.0\.0\.0:(\d+)/.match(File.read log_path)
        port = matches[1]

        
      }

      server_thread.join
      client_thread.join

      puts "done"
    end

  end
end

