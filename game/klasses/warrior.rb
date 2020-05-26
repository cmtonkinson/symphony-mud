module Talent
  class Warrior < Klass
    def initialize
      @name        = 'warrior'
      @health_rate = 4
      @mana_rate   = 0.3

      ################################# level  trains  difficulty  stamina  mana  success_rate
      # Offense
      add_ability SecondAttackSkill.new 25,    2,      3,          1,       0,    90
      add_ability ThirdAttackSkill.new  50,    3,      3,          2,       0,    80
      add_ability FourthAttackSkill.new 75,    4,      3,          3,       0,    70
      add_ability KickSkill.new         30,    2,      2,          4,       0,    90

      # Defense
      add_ability DuckSkill.new         11,    1,      3,          4,       0,    10
      add_ability BlockSkill.new        16,    1,      3,          3,       0,    20
      add_ability ParrySkill.new        32,    2,      3,          4,       0,    15
      add_ability RiposteSkill.new      54,    3,      5,          5,       0,    20

      # Dependency graph
      add_prerequisite SecondAttackSkill, ThirdAttackSkill
      add_prerequisite ThirdAttackSkill,  FourthAttackSkill
      add_prerequisite BlockSkill,        ParrySkill
      add_prerequisite SecondAttackSkill, RiposteSkill
      add_prerequisite ParrySkill,        RiposteSkill
    end
  end
end
