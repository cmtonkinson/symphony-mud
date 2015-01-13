
#include "ability-skills.h"
#include "klass.h"

Warrior::Warrior(void) {
  _name = "warrior";

  // Construct all abilities for the klass.
  Ability* second_strike = new SecondStrikeSkill(25);
  Ability* third_strike  = new ThirdStrikeSkill(50);
  Ability* fourth_strike = new FourthStrikeSkill(75);
  Ability* block         = new BlockSkill(15);
  Ability* parry         = new ParrySkill(45);
  Ability* riposte       = new RiposteSkill(65);

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
