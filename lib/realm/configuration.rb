module Realm

  class Configuration

    attr_accessor :error_string

    def initialize
      @error_string = "[{RERROR{x]"
    end

  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
