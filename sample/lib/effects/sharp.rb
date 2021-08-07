module Realm
  module Effects
    class Sharp < Effect
      def initialize(id = nil, props = {})
        super
        @name      = :sharp
        @valid_for = [Realm::Item]
        allow_uses 10

        @modifiers = {
          hit_bonus: 1,
          damage_bonus: 1
        }
      end

      def _apply
        target.register_effect :strike, self
      end

      def _expire
        target.unregister_effect :strike, self
      end
    end
  end
end
