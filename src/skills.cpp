
#include "skills.h"
#include "creature.h"

bool SecondStrikeSkill::execute(Creature* creature) const {
  creature->strike();
  return true;
}

bool ThirdStrikeSkill::execute(Creature* creature) const {
  creature->strike();
  return true;
}

bool FourthStrikeSkill::execute(Creature* creature) const {
  creature->strike();
  return true;
}

bool BlockSkill::execute(Creature* creature) const {
  return true;
}

bool ParrySkill::execute(Creature* creature) const {
  return true;
}

bool RiposteSkill::execute(Creature* creature) const {
  return true;
}
