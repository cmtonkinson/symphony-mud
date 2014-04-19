module Base

  class Configuration
    attr_accessor :sleep_interval, :command_sets

    def initialize
      @sleep_interval = 1
      @command_sets   = []
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
