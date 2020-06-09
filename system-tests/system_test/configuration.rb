module SystemTest
  class Configuration
    attr_accessor :test_registry

    def initialize
      # This is a flat list of all tests.
      @test_registry = []
    end

    def register_test(classname)
      @test_registry << classname
    end

    def self.configuration
      @configuration ||= Configuration.new
    end

    def self.configure
      yield configuration if block_given?
    end
  end
end
