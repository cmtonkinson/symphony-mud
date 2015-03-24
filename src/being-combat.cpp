
#include "ability.hpp"
#include "being.hpp"
#include "command.hpp"
#include "display.hpp"
#include "item-types.hpp"
#include "os.hpp"
#include "skills.hpp"
#include "stats.hpp"
#include "world.hpp"

void Being::formGroup(void) {
  group(new Group());
  group()->add_member(this);
  group()->leader(this);
  return;
}

void Being::ungroup(void) {
  if (group()->size() == 1) return;
  if (this == group()->leader()) {
    group()->send("$p has disbanded the company.\n", this);
    std::set<Being*> temp(group()->members());
    for (std::set<Being*>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
      if (*iter != this) (*iter)->ungroup();
    }
  }
  group()->send("$p leaves the group.\n", this);
  group()->remove_member(this);
  formGroup();
  send("You leave the group. Good luck on your own!\n");
  return;
}

void Being::add_opponent(Being* opponent, bool reciprocal) {
  // Clone each pointer set for clarity.
  std::set<Being*> groupies(group()->members());
  std::set<Being*> other_groupies(opponent->group()->members());
  // Everyone should be tracking the entire opposing Group, and scheduled to attack.
  for (std::set<Being*>::iterator iter = groupies.begin(); iter != groupies.end(); ++iter) {
    (*iter)->opponents().insert(other_groupies.begin(), other_groupies.end());
    (*iter)->scheduleAttack();
  }
  // Should the opponents be tracking you?
  if (reciprocal) opponent->add_opponent(this, false);
  return;
}

void Being::remove_opponent(Being* opponent, bool reciprocal) {
  opponents().erase(opponent);
  if (reciprocal) opponent->remove_opponent(this, false);
  return;
}

void Being::scheduleAttack(void) {
  // If there's already a Job scheduled, don't add another.
  if (_next_attack) return;
  // Create the Job, and keep a pointer for future reference.
  _next_attack = new Job(time(NULL) + 2, this, &Being::attack, "Being::attack");
  // Add it to the master schedule.
  World::Instance().schedule()->add(_next_attack);
  return;
}

bool Being::attack(Job* job) {
  // Clear the Job pointer so a new attack can be scheduled. (The Schedule will automatically
  // delete the Job when it fires, so the pointer will be invalid once this method returns anyway).
  _next_attack = NULL;
  // Aquire a target.
  acquireTarget();
  if (_target == NULL) {
    // If no valid target was found, the opponents set should be empty, but we want to be explicit
    // about what happens (and also to ensure that the attack pointer gets cleared).
    peace();
    return false;
  }
  // Make the strike. Second, third, and fourth strike cascade - e.g. if the first strike didn't
  // succeed, then don't even attempt a second.
  if (strike()) {
    if (invokeIfLearned(SECOND_STRIKE)) {
      if (invokeIfLearned(THIRD_STRIKE)) {
        invokeIfLearned(FOURTH_STRIKE);
      }
    }
  }
  // Go another round. Even if the current target is dead, there may be remaining Group members.
  scheduleAttack();
  // Is it over?
  if (_target->isDead()) _target->whatHappensWhenIDie();
  else _target->announceStatus();
  // Clear the target pointer, just in case.
  _target = NULL;
  // Must return bool per the Job interface.
  return true;
}

void Being::acquireTarget(void) {
  Being* target     = NULL;
  bool valid_target = false;
  _target           = NULL;
  while (!opponents().empty()) {
    target       = *opponents().begin();
    valid_target = true;
    // You can't attack someone in a different room.
    if (target->room() != room()) valid_target = false;
    // You can't attack someone you can't see.
    if (canSee(target) != Being::SEE_NAME) valid_target = false;
    // Either return the target, or remove it from the opponent list.
    if (valid_target) {
      _target = target;
      return;
    } else {
      // Unlink these two as targets.
      remove_opponent(target);
      continue;
    }
  }
  return;
}

// secondary should only be set while dual wielding, during the off-hand strike
bool Being::strike(Item* secondary) {
  int damage = 0;
  std::string weapon_damage;
  Item* weapon = nullptr;
  // Is the target still alive?
  if (_target->isDead()) return false;
  // Can we even move?
  if (!deplete_stamina(1)) return false;
  // Will we even land the hit?
  // Note: return true because e.g. a block doesn't preclude a second strike
  if (_target->evade(this)) return true;
  // Which weapon are we using?
  weapon = secondary != nullptr ? secondary : primary();
  // Initial damage calculation (based on the attacker).
  damage = level() * strength();
  // Adjust damage (based on the defender).
  damage -= _target->level() * _target->constitution() / 2 - _target->armor();
  // Ensure that SOME damage gets dealt.
  if (damage < 1) damage = 1;
  // Tell the world.
  weapon_damage = (weapon && weapon->isWeapon()) ? weapon->weapon()->verb().string() : "punch";
  weapon_damage.append(" ").append(Display::formatDamage(damage));
  send("Your %s %s!\n", weapon_damage.c_str(), _target->name());
  _target->send("%s's %s you!\n", name(), weapon_damage.c_str());
  room()->send_cond("$p's $s $C!", this, (void*)weapon_damage.c_str(), _target, Room::TO_NOTVICT, true);
  // Deal the pain.
  _target->takeDamage(damage, this);
  // Can we hit them again?
  if (secondary == nullptr) invokeIfLearned(DUAL_WIELD);
  return true;
}

bool Being::evade(Being* striker) {
  Ability* skill = nullptr;
  std::vector<Ability*> evasion_skills;

  // What evasion methods are available?
  if ((skill = learned().find_skill(BLOCK)) != nullptr) evasion_skills.push_back(skill);
  if (primary() || secondary()) {
    if ((skill = learned().find_skill(PARRY)) != nullptr) evasion_skills.push_back(skill);
  }
  if (evasion_skills.empty()) return false;

  // 75% chance evasion will be attempted
  // if (Math::rand(1, 4) <= 3) return false;

  // Select an evasion Skill.
  skill = evasion_skills[Math::rand(0, evasion_skills.size() - 1)];
  return skill->execute(this, striker, nullptr);
}

void Being::takeDamage(int damage, Being* damager) {
  if (damager) add_opponent(damager);
  health(health() - damage);
  deplete_stamina(1);
  if (level() > DEMIGOD && health() < 1) health(1);
  if (health() < 1) die(damager);
  return;
}

void Being::die(Being* killer) {
  unsigned experience = 0;
  // Stop fighting. Ain't nobody got time for zombies.
  peace();
  // Set the death flag.
  action().set(DEAD);
  // Reset stats.
  health(1);
  mana(1);
  stamina(0);
  // Announce the death.
  send("\n\nYou are {RDEAD{x!!!\n\n");
  room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, Room::TO_NOTVICT);
  if (isAvatar()) {
    if (killer) {
      INFO_(this, "killed by %s", killer->ident().c_str())
    } else {
      INFO(this, "died")
    }
  }
  // Who's responsible?
  if (killer) {
    experience = killer->level() * 10;
    if (killer->group()->size() > 1) {
      for (std::set<Being*>::iterator iter = killer->group()->members().begin(); iter != killer->group()->members().end(); ++iter) {
        (*iter)->awardExperience(experience / killer->group()->size());
      }
    } else {
      killer->awardExperience(experience);
    }
  }
  return;
}

void Being::peace(void) {
  // Stop tracking opponents.
  while (!opponents().empty()) remove_opponent(*opponents().begin());
  // Don't attack anymore.
  if (_next_attack) {
    World::Instance().schedule()->remove(_next_attack);
    _next_attack = NULL;
  }
  return;
}

void Being::awardExperience(unsigned experience) {
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

void Being::gainLevel(void) {
  unsigned new_level = level() + 1;
  unsigned new_tnl   = Stats::polynomial(new_level, BASE_TNL, targetTNL());
  int health_boost   = Stats::logistic(new_level, MIN_HEALTH_GAIN, targetHealth());
  int mana_boost     = Stats::logistic(new_level, MIN_MANA_GAIN, targetMana());
  int trains_boost   = 1;
  // Adjust stats.
  level(new_level);
  maxHealth(maxHealth() + health_boost);
  maxMana(maxMana() + mana_boost);
  trains(trains() + trains_boost);
  // Clear TNL if the Being just hero'd
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
  send("You gain {B%u{x training point.\n", trains_boost);
  VERBOSE_(this, "achieved level %u", level());
  if (level() < HERO) {
    send("You have {Y%u{x experience to your next level.\n\n", tnl());
  } else if (isAvatar()) {
    group()->send("$p has grown a level!\n", this, NULL, NULL);
  }
  return;
}

bool Being::autoassist(void) const {
  return true;
}

void Being::heal(void) {
  health(maxHealth());
  mana(maxMana());
  stamina(MAX_STAMINA);
  return;
}

void Being::announceStatus(void) {
  double health_percent = healthPercent();
  std::string message;

  if (health_percent > 98.0) {
    message = "$p looks {Gfantastic{x.";
  } else if (health_percent > 90.0) {
    message = "$p is in {Bexcellent{x condition.";
  } else if (health_percent > 70.0) {
    message = "$p is in {Mgreat{x condition.";
  } else if (health_percent > 50.0) {
    message = "$p is in {Cgood{x condition.";
  } else if (health_percent > 35.0) {
    message = "$p looks a little {cworn{x.";
  } else if (health_percent > 20.0) {
    message = "$p looks pretty {mworn{x.";
  } else if (health_percent > 10.0) {
    message = "$p looks very {ybattered{x.";
  } else if (health_percent > 5.0) {
    message = "$p is pretty {rbeat up{x.";
  } else {
    message = "$p looks {wterrible{x.";
  }

  room()->send_cond(message.c_str(), this, nullptr, nullptr, Room::TO_NOTVICT);
  return;
}

double Being::healthPercent(void) const {
  return 100.0 * health() / maxHealth();
}

unsigned Being::targetHealth(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 2000;
    case ROGUE:   return 3500;
    case WARRIOR: return 4000;
  }
  return 1;
}

unsigned Being::targetMana(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 3000;
    case ROGUE:   return 500;
    case WARRIOR: return 300;
  }
  return 1;
}

unsigned Being::targetMovement(void) const {
  return 1000;
}

unsigned Being::targetTNL(void) const {
  return TARGET_TNL;
}

void Being::resetStats(void) {
  level(1);
  exp(BASE_EXP);
  tnl(BASE_TNL);
  maxHealth(BASE_HEALTH);
  health(BASE_HEALTH);
  maxMana(BASE_MANA);
  mana(BASE_MANA);
  stamina(MAX_STAMINA);
  return;
}
