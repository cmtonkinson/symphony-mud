module Talent
  class Warrior < Klass

    def initialize
      @name        = "warrior"
      @health_rate = 4
      @mana_rate   = 0.3
 
      ################################### level  trains  difficulty  stamina   successRate
      # Offense
      add_ability Skill::SecondAttack.new 25,    2,      3,          1,        80
      add_ability Skill::ThirdAttack.new  50,    3,      3,          2,        80
      add_ability Skill::FourthAttack.new 75,    4,      3,          3,        80
      add_ability Skill::Whirlwind.new    60,    4,      4,          10,       95

      # Defense
      add_ability Skill::Block.new        15,    1,      3,          2,        25
      add_ability Skill::Parry.new        45,    1,      3,          2,        40

      # Dependency graph
      add_dependency Skill::SecondAttack, Skill::ThirdAttack
      add_dependency Skill::ThirdAttack,  Skill::FourthAttack
      add_dependency Skill::Block,        Skill::Parry

    end

  end
end

