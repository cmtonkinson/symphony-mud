module Talent
  class Mage < Klass
    def initialize
      @name        = 'mage'
      @health_rate = 0.3
      @mana_rate   = 4

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Skills
      add_ability SecondAttackSkill.new 60,    3,      4,          3,       0,    80

      # Spells
      add_ability FrostSpell.new        5,     2,      3,          3,       5,    90
    end
  end
end
