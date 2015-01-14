
#include "skills.h"
#include "spells.h"
#include "klass.h"

Mage::Mage(void) {
  _name = "mage";

  // Construct sklls for the klass.
  Ability* second_strike = new SecondStrikeSkill(70, 4, 2);
  // Construct spells for the klass.
  Spell* invisibility        = new InvisibilitySpell(25, 1, 25);
  Spell* detect_invisibility = new DetectInvisibilitySpell(60, 3, 75);
  Spell* frost               = new FrostSpell(5, 1, 15);
  Spell* flame               = new FlameSpell(5, 1, 15);
  Spell* spark               = new SparkSpell(5, 1, 15);

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
