
#include "creature.h"
#include "display.h"
#include "object-weapon.h"
#include "stats.h"
#include "world.h"

void Creature::add_opponent(Creature* opponent) {
  _opponents.insert(opponent);
  return;
}

void Creature::remove_opponent(Creature* opponent) {
  _opponents.erase(opponent);
  return;
}

bool Creature::is_opponent(Creature* creature) {
  return _opponents.find(creature) != _opponents.end();
}

void Creature::ungroup(void) {
  if (group()->size() == 1) return;
  if (this == group()->leader()) {
    group()->send("$p has disbanded the company.\n", this);
    std::set<Creature*> temp(group()->members());
    for (std::set<Creature*>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
      if (*iter != this) (*iter)->ungroup();
    }
  }
  group()->send("$p leaves the group.\n", this);
  group()->remove_member(this);
  group(new Group());
  group()->add_member(this);
  group()->leader(this);
  send("You leave the group. Good luck on your own!\n");
  return;
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // Aquire a target.
  if ((target = aquireTarget()) == NULL) {
    peace();
    return false;
  }
  // Make the strike.
  strike(target);
  // Schedule next attack.
  scheduleAttack();
  // Get everyone involved.
  escalate(target);
  // Must return bool per the Job interface.
  return true;
}

Creature* Creature::aquireTarget(void) {
  // TODO threat/aggro calculations
  for (std::set<Creature*>::iterator iter = opponents().begin(); iter != opponents().end(); ++iter) {
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

void Creature::escalate(Creature* target) {
  // Make sure the other guy gets a turn.
  target->add_opponent(this);
  target->scheduleAttack();
  // Get the whole group involved.
  for (std::set<Creature*>::iterator iter = group()->members().begin(); iter != group()->members().end(); ++iter) {
    // Skip the initial belligerent.
    if (*iter == this) continue;
    // Respect the members' auto-assist setting.
    if ((*iter)->autoassist()) {
      (*iter)->add_opponent(target);
      // Could be that this member was already fighting someone. If not, make sure they start.
      if (!(*iter)->nextAttack()) {
        (*iter)->scheduleAttack();
      }
    }
  }
  return;
}

void Creature::scheduleAttack(void) {
  _next_attack = time(NULL) + rand() % 5;
  World::Instance().schedule()->add(new Job(_next_attack, this, &Creature::attack));
  return;
}

void Creature::peace(void) {
  for (std::set<Creature*>::iterator iter = opponents().begin(); iter != opponents().end(); ++iter) {
    (*iter)->remove_opponent(this);
  }
  opponents().clear();
  _next_attack = 0;
  return;
}

void Creature::takeDamage(int damage, Creature* damager) {
  health(health() - damage);
  if (level() > DEMIGOD && health() < 1) health(1);
  if (health() < 1) die(damager);
  return;
}

void Creature::die(Creature* killer) {
  unsigned experience = 0;
  // Reset stats.
  health(1);
  mana(1);
  movement(1);
  // Remove from combat.
  peace();
  // Announce the death.
  send("\n\nYou are {RDEAD{x!!!\n\n");
  room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, TO_NOTVICT);
  // Who's responsible?
  if (killer) {
    experience = killer->level() * 10;
    if (killer->group()->size() > 1) {
      for (std::set<Creature*>::iterator iter = killer->group()->members().begin(); iter != killer->group()->members().end(); ++iter) {
        (*iter)->awardExperience(experience / killer->group()->size());
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
      if (level() >= HERO) return;
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
  // Clear TNL if the Creature just hero'd
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
  // TODO make this togglable
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
