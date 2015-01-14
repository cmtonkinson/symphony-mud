
#include "skills.h"
#include "creature.h"

bool Skill::invoke(Creature* creature) const {
  // A non-positive stamina value for a Skill indicates that execute() will deduct stamina for the
  // Skill dynamically. A positive value, alternatively, indicates that the stamina cost for a Skill
  // is a preset fixed value, and can be automatically deducted here to DRY out the respective
  // execute().
  if (stamina() > 0) creature->deplete_stamina(stamina());
  // Proxy to the individual details for the Skill.
  return execute(creature);
}

bool SecondStrikeSkill::execute(Creature* creature) const {
  return true;
}

bool ThirdStrikeSkill::execute(Creature* creature) const {
  return true;
}

bool FourthStrikeSkill::execute(Creature* creature) const {
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
