module Talent
  class Cleric < Klass
    def initialize
      @name        = 'cleric'
      @health_rate = 3
      @mana_rate   = 3

      ################################# level  trains  difficulty  stamina  mana  success_rate
      add_ability HealSpell.new         5,     2,      3,          3,       5,    90
    end
  end
end

