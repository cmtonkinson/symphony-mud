
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

void Creature::hit(Creature* target) {
  add_opponent(target);
  attack(NULL);
  return;
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // Since it will be deleted when this method returns anyway, we can use it for tracking state. As
  // such, we need to clear it now so another can be scheduled (see scheduleAttack()).
  _next_attack = NULL;
  // Aquire a target.
  if ((target = acquireTarget()) == NULL) {
    peace();
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
  if (health() < 1) {
    die(damager);
  } else if (damager) {
    // Launch the counter-offensive.
    add_opponent(damager);
    scheduleAttack();
    // The damager will attack, as will their Group.
    damager->escalate(this);
  }
  // Engage the rest of the Group.
  escalate(damager);
  return;
}

void Creature::scheduleAttack(void) {
  // Along with clearing the pointer in attack(), this will prevent multiple attack Jobs from
  // being created simultaneously.
  if (nextAttack()) return;
  // Create the new Job.
  time_t time_of_attack = time(NULL) + 2;
  _next_attack          = new Job(time_of_attack, this, &Creature::attack);
  // Schedule it.
  World::Instance().schedule()->add(nextAttack());
  return;
}

void Creature::escalate(Creature* opponent) {
  // Fight to the death!
  if (!isDead()) scheduleAttack();
  // Get the whole group involved.
  for (std::set<Creature*>::iterator iter = group()->members().begin(); iter != group()->members().end(); ++iter) {
    // No zombies.
    if ((*iter)->isDead()) continue;
    // Skip the initial belligerent.
    if (*iter == this) continue;
    // Don't even bother if they're not here.
    if ((*iter)->room() != opponent->room()) continue;
    // Respect the members' auto-assist setting.
    if ((*iter)->autoassist()) {
      (*iter)->add_opponent(opponent);
      (*iter)->scheduleAttack();
    }
  }
  return;
}

void Creature::peace(void) {
  // Stop opponents from tracking us.
  for (std::set<Creature*>::iterator iter = opponents().begin(); iter != opponents().end(); ++iter) {
    (*iter)->remove_opponent(this);
  }
  // Stop tracking opponents.
  while (!opponents().empty()) remove_opponent(*opponents().begin());
  // We can't have zombies running around punching every Tom, Dick, and Harry.
  if (nextAttack()) World::Instance().schedule()->remove(nextAttack());
  _next_attack = NULL;
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
  // Remove from combat.
  peace();
  // If it's a one-shit kill (or various other unlikely but possible edge cases) we won't already
  // be tracking the killer. That means that peace() will leave dangling pointers causing a crash.
  // Better safe than sorry.
  if (killer) {
    remove_opponent(killer);
    killer->remove_opponent(this);
  }
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
