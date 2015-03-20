
#include "being.h"
#include "skills.h"

bool SecondStrikeSkill::execute(Being* being) const {
  being->strike();
  return true;
}

bool ThirdStrikeSkill::execute(Being* being) const {
  being->strike();
  return true;
}

bool FourthStrikeSkill::execute(Being* being) const {
  being->strike();
  return true;
}

bool BlockSkill::execute(Being* being) const {
  return true;
}

bool ParrySkill::execute(Being* being) const {
  return true;
}

bool RiposteSkill::execute(Being* being) const {
  return true;
}
