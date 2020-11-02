module Realm
  class RageEffect < Effect
    def initialize(id = nil, props = {})
      super
      @name            = 'rage'
      @announce_apply  = true
      @announce_expire = true
      @duration        = 300

      @modifiers = {
        'strength': 1,
        'dexterity': 1,
        'constitution': -2
      }
    end
  end
end
