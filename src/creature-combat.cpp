
#include "creature.h"
#include "display.h"
#include "object-weapon.h"
#include "stats.h"
#include "world.h"


bool Creature::inCombat(void) {
  return group()->in_combat() && _next_attack > 0;
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // No-op if not in combat.
  if (!inCombat()) return false;
  // Aquire a target.
  if ((target = aquireTarget()) == NULL) return false;
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
  // TODO threat/aggro calculations
  for (std::set<Creature*>::iterator iter = group()->opponents().begin(); iter != group()->opponents().end(); ++iter) {
    // You need to be able to see the target.
    if (canSee(*iter) != SEE_NAME) continue;
    // They must be in the same Room.
    if ((*iter)->room() != room()) continue;
    // Otherwise, looks like we have our winner!
    return *iter;
  }
  return NULL;
}

void Creature::strike(Creature* target) {
  int damage = 0;
  std::string weapon_noun;
  std::string damage_verb;
  Object* object = NULL;

  // Initial damage calculation (based on the attacker).
  damage = level() * strength();
  // Adjust damage (based on the defender).
  damage -= target->level() * target->constitution() / 2 - target->armor();
  // Ensure that SOME damage gets dealt.
  if (damage < 1) damage = 1;

  // Tell the world.
  weapon_noun = "punch";
  object = primary();
  if (object && object->isWeapon()) weapon_noun = object->weapon()->verb().string();
  damage_verb = Display::formatDamage(damage);
  send("Your %s %s %s!\n", weapon_noun.c_str(), damage_verb.c_str(), target->identifiers().shortname().c_str());
  target->send("%s's %s %s you!\n", identifiers().shortname().c_str(), weapon_noun.c_str(), damage_verb.c_str());
  room()->send_cond("$p's punch $s $C!", this, (void*)damage_verb.c_str(), target, TO_NOTVICT, true);

  // Deal the pain. This is done last for two reasons:
  // 1. Because Mob::die() invokes self-deletion.
  // 2. Because a death notification should appear below the attack that caused it.
  target->takeDamage(damage, this);

  return;
}

/*
 * Ensure that each member of the target Group is tracking this Creature as an opponent. Any new
 * adversaries should schedule their first attack.
 */
void Creature::escalate(Group* group) {
  group->add_opponent(this);
  for (std::set<Creature*>::iterator iter = group->members().begin(); iter != group->members().end(); ++iter) {
    if (!(*iter)->nextAttack() && (*iter)->autoassist()) (*iter)->scheduleAttack();
  }
  return;
}

void Creature::scheduleAttack(bool now) {
  _next_attack = time(NULL);
  if (!now) _next_attack += rand() % 5;
  World::Instance().schedule()->add(new Job(_next_attack, this, &Creature::attack));
  return;
}

void Creature::stopAttacking(void) {
  _next_attack = 0;
  return;
}

void Creature::takeDamage(int damage, Creature* damager) {
  health(health() - damage);
  if (health() < 1) die(damager);
  return;
}

void Creature::die(Creature* killer) {
  unsigned experience = 0;
  // Stop attacking.
  _next_attack = 0;
  // Reset stats.
  health(1);
  mana(1);
  movement(1);
  // Remove from combat.
  for (std::set<Creature*>::iterator iter = group()->opponents().begin(); iter != group()->opponents().end(); ++iter) {
    (*iter)->group()->opponents().erase(this);
  }
  // Announce the death.
  send("\n\nYou are {RDEAD{x!!!\n\n");
  room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, TO_NOTVICT);
  // Who's responsible?
  if (killer) {
    experience = killer->level() * 10;
    if (killer->isGrouped()) {
      for (std::set<Creature*>::iterator iter = killer->group()->members().begin(); iter != killer->group()->members().end(); ++iter) {
        (*iter)->awardExperience(experience / killer->group()->members().size());
      }
    } else {
      killer->awardExperience(experience);
    }
  }
  // Leave a corpse.
  room()->send("[TODO - leave a corpse with loot]\n");
  return;
}

void Creature::awardExperience(unsigned experience) {
  if (level() >= HERO) return;
  send("You gain {Y%u{x experience points!\n", experience);
  exp(exp() + experience);
  while (experience > 0) {
    if (experience < tnl()) {
      tnl(tnl() - experience);
      break;
    } else {
      experience -= tnl();
      gainLevel();
    }
  }
  return;
}

void Creature::gainLevel(void) {
  unsigned new_level = level() + 1;
  unsigned new_tnl   = Stats::polynomial(new_level, BASE_TNL, targetTNL());
  int health_boost   = Stats::logistic(new_level, MIN_HEALTH_GAIN, targetHealth());
  int mana_boost     = Stats::logistic(new_level, MIN_MANA_GAIN, targetMana());
  int movement_boost = Stats::logistic(new_level, MIN_MOVEMENT_GAIN, targetMovement());
  // Adjust stats.
  level(new_level);
  maxHealth(maxHealth() + health_boost);
  maxMana(maxMana() + mana_boost);
  maxMovement(maxMovement() + movement_boost);
  if (level() < HERO) {
    tnl(new_tnl);
  } else {
    tnl(0);
  }
  // Heal.
  heal();
  // Notify.
  send("\n\nCONGRATULATIONS! You grow to level {G%u{x!\n", level());
  send("You gain {G%u{x health points.\n", health_boost);
  send("You gain {C%u{x mana points.\n", mana_boost);
  send("You gain {M%u{x movement points.\n", movement_boost);
  if (level() < HERO) {
    send("You have {Y%u{x experience to your next level.\n\n", tnl());
  }
  group()->send("$p has grown a level!\n", this, NULL, NULL);
  return;
}

bool Creature::autoassist(void) const {
  return true;
}

void Creature::heal(void) {
  health(maxHealth());
  mana(maxMana());
  movement(maxMovement());
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

unsigned Creature::targetMovement(void) const {
  return 1000;
}

unsigned Creature::targetTNL(void) const {
  // TODO: fill with crap from character creation
  return TARGET_TNL;
}
