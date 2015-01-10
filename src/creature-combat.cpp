
#include "creature.h"
#include "display.h"
#include "object-weapon.h"
#include "stats.h"
#include "world.h"

void Creature::formGroup(void) {
  group(new Group());
  group()->add_member(this);
  group()->leader(this);
  return;
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
  formGroup();
  send("You leave the group. Good luck on your own!\n");
  return;
}

void Creature::hit(Creature* target) {
  attack(NULL);
  return;
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // Aquire a target.
  if ((target = acquireTarget()) == NULL) {
    return false;
  }
  // Make the strike.
  strike(target);
  // Is it over?
  if (target->isDead()) {
    target->whatHappensWhenIDie();
    return true;
  }
  // Must return bool per the Job interface.
  return true;
}

Creature* Creature::acquireTarget(void) {
  return NULL;
}

void Creature::strike(Creature* target) {
  int damage = 0;
  std::string weapon_damage;
  Object* object = NULL;

  // Initial damage calculation (based on the attacker).
  damage = level() * strength();
  // Adjust damage (based on the defender).
  damage -= target->level() * target->constitution() / 2 - target->armor();
  // Ensure that SOME damage gets dealt.
  if (damage < 1) damage = 1;

  // Tell the world.
  object = primary();
  weapon_damage = (object && object->isWeapon()) ? object->weapon()->verb().string() : "punch";
  weapon_damage.append(" ").append(Display::formatDamage(damage));

  send("Your %s %s!\n", weapon_damage.c_str(), target->name());
  target->send("%s's %s you!\n", name(), weapon_damage.c_str());
  room()->send_cond("$p's $s $C!", this, (void*)weapon_damage.c_str(), target, TO_NOTVICT, true);

  // Deal the pain.
  target->takeDamage(damage, this);

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
  // Set the death flag.
  action().set(DEAD);
  // Reset stats.
  health(1);
  mana(1);
  movement(1);
  // Announce the death.
  send("\n\nYou are {RDEAD{x!!!\n\n");
  room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, TO_NOTVICT);
  if (isAvatar() && killer && killer->isAvatar()) {
    World::Instance().bigBrother(this, ADMIN_BIGBRO_DEATHS, "%s has killed %s.\n", killer->name(), name());
  }
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
  // TODO Leave a corpse with loot
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
  if (isAvatar()) group()->send("$p has grown a level!\n", this, NULL, NULL);
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

void Creature::resetStats(void) {
  level(1);
  exp(BASE_EXP);
  tnl(BASE_TNL);
  maxHealth(BASE_HEALTH);
  health(BASE_HEALTH);
  maxMana(BASE_MANA);
  mana(BASE_MANA);
  maxMovement(BASE_MOVEMENT);
  movement(BASE_MOVEMENT);
  return;
}
