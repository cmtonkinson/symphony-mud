
#include "attack.hpp"
#include "being.hpp"
#include "display.hpp"
#include "item.hpp"
#include "item-types.hpp"
#include "os.hpp"
#include "room.hpp"
#include "skills.hpp"

///////////////////////////////////////////////////////////////////////////////
// ATTACKS
///////////////////////////////////////////////////////////////////////////////
bool SecondStrikeSkill::execute(Being* being) const {
  return Math::percent_chance(80) && being->strike();
}

bool ThirdStrikeSkill::execute(Being* being) const {
  return Math::percent_chance(80) && being->strike();
}

bool FourthStrikeSkill::execute(Being* being) const {
  return Math::percent_chance(80) && being->strike();
}

bool DualWieldSkill::execute(Being* being) const {
  Item* weapon = being->secondary();

  // Need an off-hand weapon to dual strike.
  if (weapon == nullptr || !weapon->isWeapon()) return false;
  // Make the hit.
  if (Math::percent_chance(75)) {
    return being->strike(weapon);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
// EVASION
///////////////////////////////////////////////////////////////////////////////
bool BlockSkill::execute(Being* being) const {
  if (_target_being == nullptr) {
    ERROR(being, "block without target")
    return false;
  }

  if (Math::percent_chance(40)) {
    being->send("You block %s's attack!\n", _target_being->name());
    _target_being->send("%s blocks your attack!\n", being->name());
    being->room()->send_cond("$p blocks $c's attack!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
    return true;
  }

  return false;
}

bool ParrySkill::execute(Being* being) const {
  Item* primary   = being->primary();
  Item* secondary = being->secondary();
  Item* weapon    = nullptr;
  Ability* skill  = nullptr;

  if (_target_being == nullptr) {
    ERROR(being, "parry without target")
    return false;
  }

  if (primary == nullptr && secondary == nullptr) {
    ERROR(being, "parry without weapon")
    return false;
  }

  if (primary != nullptr && secondary != nullptr) {
    weapon = Math::percent_chance(50) ? primary : secondary;
  } else {
    weapon = primary != nullptr ? primary : secondary;
  }

  if (!weapon->isWeapon()) return false;

  if (Math::percent_chance(40)) {
    being->send("You parry %s's attack with %s!\n", _target_being->name(), weapon->name());
    _target_being->send("%s parries your attack with %s!\n", being->name(), weapon->name());
    being->room()->send_cond("$p parries $c's attack with $O!\n", being, _target_being, weapon, Room::TO_NOTVICT, true);
    // Should we riposte?
    if ((skill = being->learned().find_skill(RIPOSTE)) != nullptr) skill->execute(being, _target_being, weapon);
    return true;
  }

  return false;
}

bool RiposteSkill::execute(Being* being) const {
  int damage = 0;
  std::string weapon_damage;

  if (_target_being == nullptr) {
    ERROR(being, "riposte without target being")
    return false;
  }
  if (_target_item == nullptr || !_target_item->isWeapon()) {
    ERROR(being, "riposte without target weapon")
    return false;
  }

  if (Math::percent_chance(40)) {
    // Damage
    Attack attack(being, _target_being);
    if (_target_item == being->secondary()) attack.offhand(true);
    attack.init();
    damage = attack.getDamage() * 0.5;
    _target_being->takeDamage(damage, being);
    // Output
    weapon_damage = _target_item->weapon()->verb().string();
    weapon_damage.append(" ").append(Display::formatDamage(damage));
    being->send("Your riposte %s %s!\n", weapon_damage.c_str(), _target_being->name());
    _target_being->send("%s's riposte %s you!\n", being->name(), weapon_damage.c_str());
    being->room()->send_cond("$p's $s $C!", being, (void*)weapon_damage.c_str(), _target_being, Room::TO_NOTVICT, true);
  }

  return true;
}

// TODO - whereas riposte is a counter-strike with the same weapon as the parry, there should be a counterattack
// skill which uses the "other" hand (either bare, or with weapon) to strike.

bool DodgeSkill::execute(Being* being) const {
  if (_target_being == nullptr) {
    ERROR(being, "dodge without target")
    return false;
  }

  if (Math::percent_chance(40)) {
    being->send("You dodge %s's attack!\n", _target_being->name());
    _target_being->send("%s dodges your attack!\n", being->name());
    being->room()->send_cond("$p dodges $c's attack!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
    return true;
  }

  return false;
}

bool DuckSkill::execute(Being* being) const {
  if (_target_being == nullptr) {
    ERROR(being, "duck without target")
    return false;
  }

  if (Math::percent_chance(40)) {
    being->send("You duck %s's attack!\n", _target_being->name());
    _target_being->send("%s ducks your attack!\n", being->name());
    being->room()->send_cond("$p ducks $c's attack!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
    return true;
  }

  return false;
}
