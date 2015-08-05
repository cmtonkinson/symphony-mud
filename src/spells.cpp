
#include "being.hpp"
#include "spells.hpp"

bool InvisibilitySpell::execute(Being* being) const {
  return true;
}

bool DetectInvisibilitySpell::execute(Being* being) const {
  return true;
}

bool FrostSpell::execute(Being* being) const {
  return true;
}

bool FlameSpell::execute(Being* being) const {
  unsigned damage = 100;

  being->send("%s smulders as small flames lick %s skin!\n", _target_being->name(), being->seePosessivePronoun(_target_being));
  _target_being->send("You smulder as small flames lick your skin!\n");
  being->room()->send_cond("$c smulders as small flames lick $r skin!\n", being, _target_being, nullptr, Room::TO_NOTVICT);

  _target_being->takeDamage(damage, being);

  return true;
}

bool SparkSpell::execute(Being* being) const {
  return true;
}
