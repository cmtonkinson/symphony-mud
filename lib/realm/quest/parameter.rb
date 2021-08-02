# Parameters exist to monitor the tasked Object for additional requirements on
# top of the primary Objective. Usually these are negative in nature (e.g. the
# Task may require that you must complete the Objective without dying, or
# without getting into a fight, etc.) although that's not axiomatic.
#
# Each Parameter type (see Konst::SPY_PARAMETERS) may define setup*, exec*,
# activate*, and deactivate* methods.

module Realm
  class Parameter < Spy
    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    def valid_symbols
      Konst::SPY_PARAMETERS.keys
    end

    ###########################################################################
    def setup_time_limit(wen)
      @state[:wen] = wen
    end

    def activate_time_limit
      @state[:job] = Mechanics::Job.new @state[:wen], self, :exec
      $world.schedule @state[:job]
    end

    def deactivate_time_limit
      @state[:job].kill
    end

    def exec_time_limit
      Symphony.logger.trace "%s time's up for %s" % [fqid_str, task.fqid_str]
      :fail
    end

    ###########################################################################
    def exec_no_dying
      Symphony.logger.trace '%s death detected for %s' % [fqid_str, task.fqid_str]
      :fail
    end

    ###########################################################################
    def exec_no_fighting
      Symphony.logger.trace '%s fighting detected for %s' % [fqid_str, task.fqid_str]
      :fail
    end
  end
end
