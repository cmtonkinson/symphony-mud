module Talent
  class Mage < Klass
    def initialize
      @name        = 'mage'
      @health_rate = 0.3
      @mana_rate   = 4

      ################################# level  trains  difficulty  stamina  mana  success_rate
      add_ability FrostSpell.new        5,     2,      3,          3,       5,    90
      add_ability HealSpell.new         5,     2,      3,          3,       5,    90
    end
  end
end
