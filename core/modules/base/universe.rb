module Base

  class Universe
    attr_accessor :clients, :server, :stop

    def initialize
      @server  = Network::Server.new
      @clients = []
      @stop    = false
    end

    def live
      loop do
        tick
        welcome self.server.accept
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
