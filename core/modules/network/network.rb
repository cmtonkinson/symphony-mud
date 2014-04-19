module Network

  class Configuration
    attr_accessor :listen_ip, :listen_port

    def initialize
      @listen_ip   = '127.0.0.1'
      @listen_port = 6500
    end
  end

  def self.configuration
    @config ||= Network::Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
