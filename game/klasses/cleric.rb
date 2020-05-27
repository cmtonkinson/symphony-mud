module Talent
  class Cleric < Klass
    def initialize(being)
      super
      @name        = 'cleric'
      @health_rate = 3
      @mana_rate   = 3

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Skills
      add_ability SecondAttackSkill.new 60,    3,      4,          3,       0,    80

      # Spells
      add_ability HealSpell.new         5,     2,      3,          3,       5,    90

      setup
    end

    def modifiers
      {
        constitution: 1,
        intelligence: 2,
        creativity: 1,
        focus: 1,
        luck: -1,
      }
    end

  end
end

