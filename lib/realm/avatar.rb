module Realm
  class Avatar < Being

    attr_accessor :user

    define_hooks :before_disconnect

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    # Proxy the following methods to the User object.
    %i(print puts).each do |meth|
      define_method meth do |format, *args|
        @user.send meth, Util::Linguist.interpret(format, *args)
      end
    end

    private

    ###########################################################################
    before_disconnect -> {
      @room.puts I18n.t("universe.room_announce_exit"), self
      Symphony.logger.info "Avatar '#{name}' #{id} logged in"
      @room.beings.remove self
    }

  end
end
