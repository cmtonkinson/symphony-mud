module Command

  class Configuration
    attr_accessor :command_registry, :command_sets

    def initialize
      # Keep track of every command created by the DSL.
      @command_registry = []
      # At runtime, which commands are available, in which groupings?
      @command_sets = {}
    end

    def register_command(classname)
      @command_registry << classname
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
