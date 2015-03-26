
#include "skills.hpp"
#include "klass.hpp"

Rogue::Rogue(void) {
  _name = "rogue";
  // Construct skills for the klass.                level   trains  difficulty   stamina
  Ability* second_strike = new SecondStrikeSkill(    40,     3,        3,          1);
  Ability* third_strike  = new ThirdStrikeSkill(     80,     4,        3,          2);
  Ability* dual_wield    = new DualWieldSkill(       30,     2,        3,          2);

  Ability* dodge         = new DodgeSkill(           10,     1,        3,          1);
  Ability* duck          = new DuckSkill(            20,     1,        3,          2);

  // Setup dependency graph.
  third_strike->add_dependency(second_strike);

  // Register all the abilities to the klass.
  _abilities.insert(second_strike);
  _abilities.insert(third_strike);
  _abilities.insert(dual_wield);
  _abilities.insert(dodge);
  _abilities.insert(duck);
  return;
}
