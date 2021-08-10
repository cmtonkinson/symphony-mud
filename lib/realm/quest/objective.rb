# Objective is meant to monitor the hero Objekt for a success event. Each
# different kind of objective (see Konst::SPY_TRIGGERS) will have a setup* and
# an exec* method defined below.
module Realm
  class Objective < Spy
    def initialize(id = nil, props = {})
      super
    end

    def valid_symbols
      Konst::SPY_TRIGGERS
    end

    def setup_visit_room(room)
      # TODO: validate this is a valid room reference (UUID, rid, etc.)
      @params = room
    end

    def exec_visit_room(being)
      # Check for both RID and UUID
      return nil unless being.room.rid == params || being.room.id == params

      Symphony.logger.trace '%s completed %s by visiting %s' % [
        task.hero.fqid_str,
        fqid_str,
        being.room.fqid_str
      ]
      :complete
    end
  end
end
