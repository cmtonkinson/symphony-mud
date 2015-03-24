
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
  return true;
}

bool SparkSpell::execute(Being* being) const {
  return true;
}
