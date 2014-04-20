module Network

  class Configuration
    attr_accessor :listen_ip, :listen_port

    def initialize
      # On which interface should the server listen for connections?
      @listen_ip   = '127.0.0.1'
      # To which port should the server bind?
      @listen_port = 6500
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
