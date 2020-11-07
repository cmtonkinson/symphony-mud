module Realm
  class Chill < Effect
    def initialize(id = nil, props = {})
      super
      @name            = :chill
      @announce_apply  = true
      @announce_expire = true
      @duration        = 30
      @recur_delay     = 5
      @valid_for       = [Realm::Being]
    end

    def _recur
      target.puts I18n.t('effect.chill.recur')
      amount = target.health.current * 0.01
      target.take_damage amount, originator
    end
  end
end
