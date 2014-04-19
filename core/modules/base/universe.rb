module Base

  class Universe
    attr_accessor :clients, :server, :stop, :reboot

    def initialize
      @server  = Network::Server.new
      @clients = []
      @stop    = false
    end

    def live
      ap ENV
      loop do
        welcome self.server.accept
        tick
        break if self.stop
      end
    end

    def die
      self.stop = true
    end

    def tick
      self.clients.each do |c|
        input = c.recv
        next if input.nil?
        operator = Command::Operator.new c, Command.configuration.command_sets[:global]
        operator.handle input
      end
      if Time.new.sec % 5 == 0
        self.clients.each { |c| c.puts "TICK #{Time.now}" }
      end
      self.clients.select { |c| c.terminate }.each do |c|
          c.close
         self.clients.delete c
      end
      if @reboot
        # save fds to disk
        name = "copyover.txt"
        file = File.open name, "a"
        self.clients.each { |c| ap c; file.puts c.get_fd }
        file.close
        # replace this process with another
        env = {
          "symphony_copyover" => "true",
        }
        command = "ruby symphony.rb"
        options = {
          :close_others => false,
        }
        pid = spawn env, command, options
        ap pid
        exit
      end
      sleep Base.configuration.sleep_interval
    end

    def welcome(client)
      return if client.nil?
      client.puts "hey yall @ #{Time.now} on socket #{client.get_fd}"
      self.clients.each { |c| c.puts "more peeps!" }
      self.clients << client
    end

  end

end
