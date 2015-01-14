
#include "spells.h"
#include "creature.h"

bool Spell::invoke(Creature* creature) const {
  // A non-positive mana value for a Spell indicates that execute() will deduct mana for the Spell
  // dynamically. A positive value, alternatively, indicates that the mana cost for a Spell is a
  // preset fixed value, and can be automatically deducted here to DRY out the respective execute().
  if (mana() > 0) creature->mana(creature->mana() - mana());
  // Proxy to the individual details for the Spell.
  return execute(creature);
}

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
