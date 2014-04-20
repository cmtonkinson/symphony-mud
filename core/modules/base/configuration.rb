module Base

  class Configuration
    attr_accessor :sleep_interval, :command_sets

    def initialize
      # Wait time (in seconds) between iterations of the primary server loop.
      @sleep_interval = 0.1
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
