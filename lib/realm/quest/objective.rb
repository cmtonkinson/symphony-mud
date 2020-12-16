module Realm
  class Objective < Spy
    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    def valid_symbols
      Konst::SPY_TRIGGERS
    end

    ###########################################################################
    def setup_visit_room(room)
      # TODO: validate this is a valid room reference (UUID, rid, etc.) and
      # convert it into the corresponding rid
      @params = room
    end

    def exec_visit_room(being)
      return nil unless being.room.rid == trigger_params

      Symphony.logger.trace '%s completed %s by visiting %s' % [
        task.tasked.fqid_str,
        fqid_str,
        being.room.fqid_str
      ]
      :complete
    end
  end
end
