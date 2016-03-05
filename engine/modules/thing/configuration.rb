module Thing

  class Configuration

    def initialize
    end

  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
