
#include "creature.h"
#include "spells.h"

bool Creature::intone(Ability* spell) {
  bool status = false;

  if (mana() < spell->mana()) {
    send("You don't have enough mana for that spell.\n");
    return false;
  }

  send("You intone the '{c%s{x' spell.\n", spell->name().c_str());
  room()->send_cond("$p mutters something strange.\n", this, this);

  status = spell->invoke(this);

  if (status) {
    send("You cast %s!\n", spell->name().c_str());
  } else {
    send("Spell failed.\n");
  }

  return true;
}
