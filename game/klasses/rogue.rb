module Talent
  class Rogue < Klass
    def initialize
      @name        = 'rogue'
      @health_rate = 2
      @mana_rate   = 0.3

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Offense
      add_ability SecondAttackSkill.new 25,    2,      3,          1,       0,    95

      # Defense
      add_ability DuckSkill.new         6,     1,      3,          4,       0,    10
      add_ability DodgeSkill.new        19,    1,      3,          4,       0,    10
      add_ability ParrySkill.new        51,    2,      3,          4,       0,    15
     end
  end
end
