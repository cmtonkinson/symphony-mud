module Core

  class World
    attr_accessor :connections

    def initialize
      @server      = Network::Server.new
      @connections = []
    end

    def spin
      loop do
        tick
        welcome @server.accept
      end
    end

    def tick
      @connections.each { |c| c.puts "TICK #{Time.now}" } if (Time.new.sec % 5 == 0)
      sleep 1
    end

    def welcome(connection)
      return if connection.nil?
      connection.puts "hey yall @ #{Time.now} on socket #{connection.get_fd}"
      @connections.each { |c| c.puts "more peeps!" }
      @connections << connection
    end

  end

end
