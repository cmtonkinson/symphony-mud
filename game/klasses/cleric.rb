module Talent
  class Cleric < Klass
    def initialize
      @name        = 'cleric'
      @health_rate = 3
      @mana_rate   = 3

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Skills
      add_ability SecondAttackSkill.new 60,    3,      4,          3,       0,    80

      # Spells
      add_ability HealSpell.new         5,     2,      3,          3,       5,    90
    end
  end
end

