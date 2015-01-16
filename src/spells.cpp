
#include "spells.h"
#include "creature.h"

bool InvisibilitySpell::execute(Creature* creature) const {
  return true;
}

bool DetectInvisibilitySpell::execute(Creature* creature) const {
  return true;
}

bool FrostSpell::execute(Creature* creature) const {
  return true;
}

bool FlameSpell::execute(Creature* creature) const {
  return true;
}

bool SparkSpell::execute(Creature* creature) const {
  return true;
}
