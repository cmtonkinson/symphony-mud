module Base

  class Configuration
    attr_accessor :sleep_interval, :command_sets
    attr_accessor :hash_salt, :hash_iterations, :hash_digest

    def initialize
      # Wait time (in seconds) between iterations of the primary server loop.
      @sleep_interval = 0.1

      # For account management
      @hash_salt       = ""
      @hash_iterations = 20_000
      @hash_digest     = OpenSSL::Digest::SHA512.new
    end
  end

  def self.configuration
    @config ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end

end
