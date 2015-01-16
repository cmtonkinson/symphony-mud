
#include "skills.h"
#include "klass.h"

Warrior::Warrior(void) {
  _name = "warrior";

  // Construct skills for the klass.                level   trains  difficulty   stamina
  Ability* second_strike = new SecondStrikeSkill(    25,     2,        3,          1);
  Ability* third_strike  = new ThirdStrikeSkill(     50,     3,        3,          2);
  Ability* fourth_strike = new FourthStrikeSkill(    75,     4,        3,          3);
  Ability* block         = new BlockSkill(           15,     1,        3,          1);
  Ability* parry         = new ParrySkill(           45,     1,        3,          1);
  Ability* riposte       = new RiposteSkill(         65,     2,        3,          1);

  // Setup dependency graph.
  third_strike->add_dependency(second_strike);
  fourth_strike->add_dependency(third_strike);
  parry->add_dependency(block);
  riposte->add_dependency(parry);
  riposte->add_dependency(second_strike);

  // Register all the abilities to the klass.
  _abilities.insert(second_strike);
  _abilities.insert(third_strike);
  _abilities.insert(fourth_strike);
  _abilities.insert(block);
  _abilities.insert(parry);
  _abilities.insert(riposte);
  return;
}
