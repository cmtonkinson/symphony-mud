module Realm
  class Sharp < Effect
    def initialize(id = nil, props = {})
      super
      @name      = 'sharp'
      @valid_for = [Realm::Item]

      @modifiers = {
        'hit_bonus': 1,
        'damage_bonus': 1
      }
    end
  end
end
