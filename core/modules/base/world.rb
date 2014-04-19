module Base

  class World
    attr_accessor :clients

    def initialize
      @server  = Network::Server.new
      @clients = []
      @stop    = false
    end

    def spin
      loop do
        tick
        welcome @server.accept
        break if @stop
      end
    end

    def stop
      @stop = true
    end

    def tick
      if Time.new.sec % 5 == 0
        @clients.each { |c| c.puts "TICK #{Time.now}" }
      end
      sleep Base.configuration.sleep_interval
    end

    def welcome(client)
      return if client.nil?
      client.puts "hey yall @ #{Time.now} on socket #{client.get_fd}"
      @clients.each { |c| c.puts "more peeps!" }
      @clients << client
    end

  end

end
