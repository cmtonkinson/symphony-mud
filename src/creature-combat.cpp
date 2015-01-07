
#include "creature.h"
#include "display.h"
#include "object-weapon.h"
#include "stats.h"
#include "world.h"


bool Creature::inCombat(void) {
  return group()->in_combat();
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // No-op if not in combat.
  if (!inCombat()) return false;
  // Aquire a target.
  target = aquireTarget();
  // Make the strike.
  strike(target);
  // Schedule next attack.
  scheduleAttack();
  // Fully involve both groups.
  escalate(target->group());
  // Must return bool per the Job interface.
  return true;
}

Creature* Creature::aquireTarget(void) {
  return *(group()->opponents().begin());
}

void Creature::strike(Creature* target) {
  unsigned int damage = 0;
  std::string weapon_noun;
  std::string damage_verb;
  Object* object = NULL;

  // Initial damage calculation (based on the attackers offense).
  damage = rand() % 250;
  // Adjust damage (based on the targets defense).
  damage -= rand() % (damage - 1);

  // Tell the world.
  weapon_noun = "punch";
  object = primary();
  if (object && object->isWeapon()) weapon_noun = object->weapon()->verb().string();
  damage_verb = Display::formatDamage(damage);
  send("Your %s %s %s!\n", weapon_noun.c_str(), damage_verb.c_str(), target->identifiers().shortname().c_str());
  target->send("%s's %s %s you!\n", identifiers().shortname().c_str(), weapon_noun.c_str(), damage_verb.c_str());
  room()->send_cond("$p's punch $s $C!", this, (void*)damage_verb.c_str(), target, TO_NOTVICT, true);

  // Deal the pain (performed last to ensure that DEAD notification appear in the proper order.
  target->takeDamage(damage);

  return;
}

void Creature::assessCombat(void) {

  return;
}

/*
 * Ensure that each member of the target Group is tracking this Creature as an opponent. Any new
 * adversaries should schedule their first attack.
 */
void Creature::escalate(Group* group) {
  group->add_opponent(this);
  for (std::set<Creature*>::iterator iter = group->members().begin(); iter != group->members().end(); ++iter) {
    if (!(*iter)->nextAttack()) (*iter)->scheduleAttack();
  }
  return;
}

void Creature::scheduleAttack(void) {
  _next_attack = time(NULL) + rand() % 5;
  World::Instance().schedule()->add(new Job(_next_attack, this, &Creature::attack));
  return;
}

void Creature::takeDamage(int damage) {
  health(health() - damage);
  if (health() < 2) {
    health(1);
    send("\n\nYou are {RDEAD{x!!!\n\n");
    room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, TO_NOTVICT);
  }
  return;
}

unsigned Creature::targetHealth(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 2000;
    case ROGUE:   return 3500;
    case WARRIOR: return 4000;
  }
  return 1;
}

unsigned Creature::targetMana(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 3000;
    case ROGUE:   return 500;
    case WARRIOR: return 300;
  }
  return 1;
}

unsigned Creature::targetMove(void) const {
  return 1000;
}

unsigned Creature::targetTNL(void) const {
  // TODO: fill with crap from character creation
  return TARGET_TNL;
}

unsigned Creature::getTNL(void) const {
  return Stats::polynomial(level(), BASE_TNL, targetTNL());
}
