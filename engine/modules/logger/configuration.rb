module Logger

  class Configuration
    attr_accessor :verbosity

    def initialize
      @verbosity = :info
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
