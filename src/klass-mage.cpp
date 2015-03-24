
#include "skills.hpp"
#include "spells.hpp"
#include "klass.hpp"

Mage::Mage(void) {
  _name = "mage";

  // Construct skills for the klass.                        level   trains  difficulty   stamina
  Ability* second_strike = new SecondStrikeSkill(            70,     4,        3,        2);

  // Construct spells for the klass.                        level   trains  difficulty   mana
  Ability* invisibility        = new InvisibilitySpell(      25,     1,        3,        25);
  Ability* detect_invisibility = new DetectInvisibilitySpell(60,     3,        3,        75);
  Ability* frost               = new FrostSpell(             5,      1,        3,        15);
  Ability* flame               = new FlameSpell(             5,      1,        3,        15);
  Ability* spark               = new SparkSpell(             5,      1,        3,        15);

  // Setup dependency graph.
  detect_invisibility->add_dependency(invisibility);

  // Register skills.
  _abilities.insert(second_strike);
  // Register spells;
  _abilities.insert(invisibility);
  _abilities.insert(detect_invisibility);
  _abilities.insert(frost);
  _abilities.insert(flame);
  _abilities.insert(spark);
  return;
}
