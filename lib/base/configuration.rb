require 'openssl'

module Base
  class Configuration
    attr_accessor :sleep_interval, :recovery_file
    attr_accessor :hash_salt, :hash_iterations, :hash_digest

    def initialize
      # Wait time (in seconds) between iterations of the primary server loop.
      @sleep_interval = 0.02

      # For account management
      @hash_salt       = ''
      @hash_iterations = 20_000
      @hash_digest     = OpenSSL::Digest::SHA512.new

      # Where to store the reboot fd => Avatar Hash?
      @recovery_file = Symphony.tmp.join 'recovery.yml'
    end
  end

  def self.configuration
    @configuration ||= Configuration.new
  end

  def self.configure
    yield configuration if block_given?
  end
end
