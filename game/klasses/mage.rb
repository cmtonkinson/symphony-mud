module Talent
  class Mage < Klass
    def initialize(being)
      super
      @name        = 'mage'
      @health_rate = 0.3
      @mana_rate   = 4

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Skills
      add_ability SecondAttackSkill.new 45,    3,      4,          3,       0,    80

      # Spells
      add_ability FrostSpell.new        5,     2,      3,          3,       5,    90

      setup
    end

    def modifiers
      {
        intelligence: 2,
        creativity: 2,
        focus: 1,
        strength: -1,
        constitution: -1,
      }
    end

 end
end
