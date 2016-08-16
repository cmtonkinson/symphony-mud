module Realm
  class Avatar < Being

    attr_accessor :user
    attr_accessor :iedit

    define_hooks :before_disconnect

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    before_disconnect -> {
      @room.puts I18n.t("universe.room_announce_exit"), self
      Symphony.logger.info "Avatar '#{name}' #{id} logged in"
      @room.beings.remove self
    }

  end
end
