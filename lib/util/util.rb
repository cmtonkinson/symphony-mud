module Util
  #############################################################################
  def self.argerr(message)
    trace = caller_locations(1, 1)[0]
    raise ArgumentError, 'ArgumentError: %s in %s at %s:%d' % [message,
                                                                  trace.base_label,
                                                                  trace.path.sub(Symphony.root.to_s + '/', ''),
                                                                  trace.lineno]
  end
end
