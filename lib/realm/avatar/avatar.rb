module Realm
  class Avatar < Being

    attr_accessor :user
    attr_accessor :iedit, :nedit, :zedit

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
      @room.beings.remove self
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

  end
end
