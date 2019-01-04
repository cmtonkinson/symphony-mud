module Talent
  class Spell < Ability
    # Valid spell categories are...
    # :charm
    # :enchantment
    # :hex
    # :curse
    # :cure
    # :harm
    # :conjuring
    # :transfiguration
    attr_reader :spell_category

    # Valid default targets are...
    # :self
    # :enemy
    attr_reader :default_target

    ###########################################################################
    def initialize(*args)
      super *args

      @spell_category = nil
      @default_target = nil
    end
  end
end
