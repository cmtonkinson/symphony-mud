module Realm
  class Avatar < Being

    attr_accessor :user
    attr_accessor :iedit, :nedit, :zedit, :redit

    define_hooks :before_disconnect

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      @force_output = false
    end

    ###########################################################################
    before_disconnect -> {
      @room.puts I18n.t("universe.room_announce_exit"), self
      Symphony.logger.info "%s logged out" % fqid_str
      @room.beings.delete self
    }

    ###########################################################################
    vivify -> {
      $world.avatars.insert self if is_a? Realm::Avatar
    }

    ###########################################################################
    def force_output!
      @force_output = true
    end

    ###########################################################################
    def test_and_clear_forced_output
      o = @force_output
      @force_output = false
      return o
    end

    ###########################################################################
    def _prompt_prefix
      if redit
        "{RREDIT{x {Y%d{x " % room.rid
      else
        ""
      end
    end

    ###########################################################################
    # Verify room edit permissions prior to room change.
    before_room_change -> (from, to) {
      if redit && from.zone != to.zone
        self.redit = false
        self.user.operators.pop
      end
    }

  end
end

