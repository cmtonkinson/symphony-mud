module Realm
  module Effects
    class Poison < Effect
      def initialize(id = nil, props = {})
        super
        @name            = :poison
        @announce_apply  = true
        @announce_expire = true
        @duration        = 300
        @recur_delay     = 15
        @valid_for       = [Realm::Being]
      end

      def _recur
        target.puts I18n.t('effect.poison.recur')
        dam = Combat::Damage.new target.health.current * 0.03
        dam.source = originator
        target.take_damage dam
      end
    end
  end
end
