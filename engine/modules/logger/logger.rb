module Logger

  VERBOSITY = {
    absurd:   0,
    verbose:  1,
    debug:    2,
    info:     3,
    warning:  4,
    error:    5,
    fatal:    6,
    silent:   7,
  }

  class Logger

    def initialize
    end

    def log(label, index, message)
      return if index < VERBOSITY[::Logger.configuration.verbosity]
      # Prepend the timestamp
      message = message.prepend Time.now.to_s + " "
      # Prepend the verbosity
      message = message.prepend label.to_s.upcase + " "
      # Proxy to the underlying Logger instance.
      _log message
    end

    VERBOSITY.each do |label, index|
      define_method :"#{label}" do |message|
        self.send "log", label, index, message
      end
    end

  end

end
