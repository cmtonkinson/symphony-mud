module Talent
  class Warrior < Klass
    def initialize
      @name        = 'warrior'
      @health_rate = 4
      @mana_rate   = 0.3
 
      ################################# level  trains  difficulty  stamina  mana  successRate
      # Offense
      add_ability SecondAttackSkill.new 25,    2,      3,          1,       0,    80
      add_ability ThirdAttackSkill.new  50,    3,      3,          2,       0,    80
      add_ability FourthAttackSkill.new 75,    4,      3,          3,       0,    80
#      add_ability WhirlwindSkill.new    60,    4,      4,          10,      0,    95

      # Defense
      add_ability DuckSkill.new         11,    1,      3,          2,       0,    15
      add_ability BlockSkill.new        16,    1,      3,          2,       0,    20
      add_ability DodgeSkill.new        25,    1,      3,          2,       0,    10
      add_ability ParrySkill.new        45,    2,      3,          2,       0,    20
      add_ability RiposteSkill.new      65,    3,      5,          3,       0,    20

      # Dependency graph
#      add_dependency SecondAttackSkill, ThirdAttackSkill
#      add_dependency ThirdAttackSkill,  FourthAttackSkill
#      add_dependency BlockSkill,        ParrySkill

    end
  end
end
