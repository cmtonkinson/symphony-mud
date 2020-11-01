module Realm
  class Poison < Effect
    ###########################################################################
    def initialize(id = nil, props = {})
      super
      @name            = 'poison'
      @announce_apply  = true
      @announce_expire = true
      @duration        = 300
    end

    ###########################################################################
    def _apply
    end

    ###########################################################################
    def _expire
    end

    ###########################################################################
    def _job
      being.puts t('effect.poison.hurt')
      being.take_damage 10, originator
    end
  end
end

