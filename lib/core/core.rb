module Core

  class Configuration
    attr_accessor :listen_ip, :listen_port

    def initialize
    end
  end

  def self.configuration
    @config ||= Core::Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
