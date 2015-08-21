
#include "being.hpp"
#include "spells.hpp"

bool Being::intone(Ability* spell, Being* being_target, Item* item_target) {
  bool status = false;

  send("You intone the '{c%s{x' spell.\n", spell->name().c_str());
  room()->send_cond("$a mutters something strange.\n", this, this);

  status = spell->invoke(this, being_target, item_target);

  return true;
}
