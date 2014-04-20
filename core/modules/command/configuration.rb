module Command

  class Configuration
    attr_accessor :command_sets

    def initialize
      # Which commands are available, in which groupings?
      @command_sets = {}
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
