module Core

  class World
    attr_accessor :connections

    def initialize
      @server      = Network::Server.new
      @connections = []
      @stop        = false
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
      @connections.each { |c| c.puts "TICK #{Time.now}" } if Time.new.sec % 5 == 0
      sleep Core.configuration.sleep_interval
    end

    def welcome(connection)
      return if connection.nil?
      connection.puts "hey yall @ #{Time.now} on socket #{connection.get_fd}"
      @connections.each { |c| c.puts "more peeps!" }
      @connections << connection
    end

  end

end
