module Realm
  module Effects
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
        dam = Combat::Damage.new target.health.current * 0.01
        dam.source = originator
        dam.ice 100
        target.take_damage dam
      end
    end
  end
end
