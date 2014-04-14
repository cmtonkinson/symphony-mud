module Core

  class Configuration
    attr_accessor :sleep_interval

    def initialize
      @sleep_interval = 1
    end
  end

  def self.configuration
    @config ||= Core::Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
