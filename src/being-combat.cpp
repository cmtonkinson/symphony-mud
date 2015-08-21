
#include "ability.hpp"
#include "being.hpp"
#include "command.hpp"
#include "display.hpp"
#include "item-types.hpp"
#include "os.hpp"
#include "skills.hpp"
#include "stats.hpp"
#include "strike-damage.hpp"
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
  _next_attack = new Job(time(NULL) + StrikeDamage::FIRST_ATTACK_DELAY, this, &Being::attack, "Being::attack");
  // Add it to the master schedule.
  World::Instance().schedule()->add(_next_attack);
  return;
}

// TODO - any type of counterattack of effect could kill `this` during offensive moves
// such as attack() - need to account for this. Maybe self-deletion is a bad strategy
bool Being::attack(Job* job) {
  // Clear the Job pointer so a new attack can be scheduled. (The Schedule will automatically
  // delete the Job when it fires, so the pointer will be invalid once this method returns anyway).
  _next_attack = nullptr;
  // Aquire a target.
  if ((_target = acquireTarget()) == nullptr) {
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
  _target = nullptr;
  // Must return bool per the Job interface.
  return true;
}

Being* Being::acquireTarget(void) {
  Being* target     = nullptr;
  bool valid_target = false;
  while (!opponents().empty()) {
    target       = *opponents().begin();
    valid_target = true;
    // You can't attack someone in a different room.
    if (target->room() != room()) valid_target = false;
    // You can't attack someone you can't see.
    if (canSee(target) != Being::SEE_NAME) valid_target = false;
    // Either return the target, or remove it from the opponent list.
    if (valid_target) {
      return target;
    } else {
      // Unlink these two as targets.
      remove_opponent(target);
      continue;
    }
  }
  return nullptr;
}

// secondary should only be set while dual wielding, during the off-hand strike
bool Being::strike(Item* secondary) {
  int damage = 0;
  std::string weapon_damage;
  const Item* weapon = nullptr;
  StrikeDamage strike_damage(this, _target);

  // Is the target still alive?
  if (_target->isDead()) return false;
  // Can we even move?
  if (!deplete_stamina(1)) return false;
  // Is this an offhand strike?
  if (secondary != nullptr) strike_damage.offhand(true);
  strike_damage.init();
  // Get the correct verb.
  if (!strike_damage.unarmed()) weapon = strike_damage.weapon()->base();
  weapon_damage = (weapon && weapon->isWeapon()) ? weapon->weapon()->verb().string() : "strike";
  // Is it a good hit?
  if (!strike_damage.hit()) {
    send("Your %s misses %s!\n", weapon_damage.c_str(), _target->name());
    _target->send("%s's %s misses you!\n", name(), weapon_damage.c_str());
    room()->send_cond("$p's $s misses $C!", this, weapon_damage.c_str(), _target, Room::TO_NOTVICT);
    return false;
  }
  // Will we land it?
  // Note: return true because e.g. a block doesn't preclude a second strike
  if (_target->evade(this)) return true;
  // Calculate the damage.
  damage = strike_damage.getDamage();
  // Tell the world.
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

  // Small chance we even attempt evasion.
  if (Math::percent_chance(70)) return false;

  // What evasion methods are available?
  if ((skill = learned().find_skill(BLOCK)) != nullptr) evasion_skills.push_back(skill);
  if (primary() || secondary()) {
    if ((skill = learned().find_skill(PARRY)) != nullptr) evasion_skills.push_back(skill);
  }
  if ((skill = learned().find_skill(DODGE)) != nullptr) evasion_skills.push_back(skill);
  if ((skill = learned().find_skill(DUCK)) != nullptr) evasion_skills.push_back(skill);
  if (evasion_skills.empty()) return false;

  // Select an evasion Skill.
  skill = evasion_skills[Math::rand(0, evasion_skills.size() - 1)];
  return skill->execute(this, striker, nullptr);
}

void Being::takeDamage(int damage, Being* damager) {
  SILLY_(this, "took %d damage from %s", damage, damager->ident().c_str());
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
    _next_attack = nullptr;
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
  if (isAvatar()) {
    VERBOSE_(this, "achieved level %u", level());
    send("\n\nCONGRATULATIONS! You grow to level {G%u{x!\n", level());
    send("You gain {G%u{x health points.\n", health_boost);
    send("You gain {C%u{x mana points.\n", mana_boost);
    send("You gain {B%u{x training point.\n", trains_boost);
    if (level() < HERO) send("You have {Y%u{x experience to your next level.\n\n", tnl());
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

  if      (health_percent > 0.98) message = "$p looks {Gfantastic{x.";
  else if (health_percent > 0.90) message = "$p is in {Bexcellent{x condition.";
  else if (health_percent > 0.70) message = "$p is in {Mgreat{x condition.";
  else if (health_percent > 0.50) message = "$p is in {Cgood{x condition.";
  else if (health_percent > 0.35) message = "$p looks a little {cworn{x.";
  else if (health_percent > 0.20) message = "$p looks pretty {mworn{x.";
  else if (health_percent > 0.10) message = "$p looks very {ybattered{x.";
  else if (health_percent > 0.5)  message = "$p is pretty {rbeat up{x.";
  else                            message = "$p looks {wterrible{x.";

  room()->send_cond(message.c_str(), this, nullptr, nullptr, Room::TO_NOTVICT);
  return;
}

double Being::healthPercent(void) const       { return 1.0 * health() / maxHealth(); }

double Being::strengthPercent(void) const     { return 1.0 * strength()     / STAT_THRESHOLD; }
double Being::dexterityPercent(void) const    { return 1.0 * dexterity()    / STAT_THRESHOLD; }
double Being::constitutionPercent(void) const { return 1.0 * constitution() / STAT_THRESHOLD; }
double Being::intelligencePercent(void) const { return 1.0 * intelligence() / STAT_THRESHOLD; }
double Being::focusPercent(void) const        { return 1.0 * focus()        / STAT_THRESHOLD; }
double Being::creativityPercent(void) const   { return 1.0 * creativity()   / STAT_THRESHOLD; }
double Being::charismaPercent(void) const     { return 1.0 * charisma()     / STAT_THRESHOLD; }
double Being::luckPercent(void) const         { return 1.0 * luck()         / STAT_THRESHOLD; }

unsigned Being::targetHealth(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 2000;
    case ROGUE:   return 3500;
    case WARRIOR: return 4000;
  }
  return 10000;
}

unsigned Being::targetMana(void) const {
  switch (pClass().number()) {
    case CLERIC:  return 2500;
    case MAGE:    return 3000;
    case ROGUE:   return 500;
    case WARRIOR: return 300;
  }
  return 5000;
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

// 1% of max but not more than the current health value
// TODO - improve to include stat adjustments, etc.
bool Being::auto_health(Job* job) {
  int add = maxHealth() / 100;
  int max = health();
  health(health() + Math::bound(add, 1, max));
  return true;
}

// 1% of max
// TODO - improve to include stat adjustments, etc.
bool Being::auto_mana(Job* job) {
  int add = maxMana() / 100;
  int max = level();
  mana(mana() + Math::bound(add, 1, max));
  return true;
}

// 1 for noobs, 20 for heroes, linear interpolation (for now)
// TODO - improve to include stat adjustments, etc.
bool Being::auto_stamina(Job* job) {
  int add = level() / 5;
  int max = MAX_STAMINA / 2.5;
  stamina(stamina() + Math::bound(add, 1, max));
  return true;
}

// Returns the current affinity for the given weapon, then adjusts it.
double Being::affinity(bool primary_hand) {
  double affinity    = 0.0;
  Item* item         = nullptr;
  ItemWeapon* weapon = nullptr;
  std::vector<double> being_affinity;
  std::vector<double> weapon_values;

  // Primary or secondary?
  if ((item = (primary_hand ? primary() : secondary())) == nullptr) {
    ERROR(this, "Being::affinity() - affinity request for null weapon");
    return affinity;
  }
  weapon = item->weapon();

  // Get the current affinity. Affinity is calculated as a distance between two
  // points on a Cartesian system in multiple dimensions. The dimensions are
  // the different affinity metrics. One point is defined by the affinity
  // values of the Being, and the other point is defined by affinity values of
  // the ItemWeapon.
  affinity = Math::distance(
    std::vector<double>(
      sizeAffinity(),
      rangeAffinity()
    ),
    std::vector<double>(
      ItemWeapon::relativeSize(weapon),
      ItemWeapon::relativeRange(weapon)
    )
  );

  // Adjust the affinity for the current weapon.
  sizeAffinity((sizeAffinity() * AFFINITY_RESISTANCE + ItemWeapon::relativeSize(weapon)) / (AFFINITY_RESISTANCE + 1.0));
  rangeAffinity((rangeAffinity() * AFFINITY_RESISTANCE + ItemWeapon::relativeRange(weapon)) / (AFFINITY_RESISTANCE + 1.0));

  return affinity;
}
