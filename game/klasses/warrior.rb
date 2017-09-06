module Talent

  klass :warrior do

    health 4000
    mana   300

    # Available skills
    #     Skill                level  trains  difficulty  stamina   successRate
    skill Skill::SecondAttack, 25,    2,      3,          1,        80
    skill Skill::ThirdAttack,  50,    3,      3,          2,        80
    skill Skill::FourthAttack, 75,    4,      3,          3,        80
    skill Skill::Whirlwind,    60,    4,      4,          10,       95

    skill Skill::Block,        15,    1,      3,          2,        25
    skill Skill::Parry,        45,    1,      3,          2,        40

    # Skill trees
    dependency Skill::SecondAttack, Skill::ThirdAttack
    dependency Skill::ThirdAttack,  Skill::FourthAttack
    dependency Skill::Block,        Skill::Parry

  end

end

