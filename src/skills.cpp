
#include "being.hpp"
#include "display.hpp"
#include "item-types.hpp"
#include "item.hpp"
#include "os.hpp"
#include "room.hpp"
#include "skills.hpp"
#include "strike.hpp"

///////////////////////////////////////////////////////////////////////////////
// ATTACKS
// FIXME - what to make of these following the Strike class update?
// NOTE - Maybe need to store last Attack in Being for proper nesting?
///////////////////////////////////////////////////////////////////////////////
bool SecondStrikeSkill::execute(Being* being) const {
  if (Math::percent_chance(20)) return false;
  Strike strike(being, _target_being, true);
  strike.nesting(1);
  return strike.strike();
}

bool ThirdStrikeSkill::execute(Being* being) const {
  if (Math::percent_chance(20)) return false;
  Strike strike(being, _target_being, true);
  strike.nesting(2);
  return strike.strike();
}

bool FourthStrikeSkill::execute(Being* being) const {
  if (Math::percent_chance(20)) return false;
  Strike strike(being, _target_being, true);
  strike.nesting(3);
  return strike.strike();
}

bool DualWieldSkill::execute(Being* being) const {
  Strike dual(being, _target_being, false);
  // Need an off-hand weapon to dual strike.
  if (dual.unarmed()) return false;
  // Moderate chance of success.
  if (Math::percent_chance(25)) return false;
  // Go for it.
  return dual.strike();
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
    being->send("You block %s's strike!\n", _target_being->name());
    _target_being->send("%s blocks your strike!\n", being->name());
    being->room()->send_cond("$a blocks $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
    return true;
  }

  return false;
}

bool ParrySkill::execute(Being* being) const {
  Item* primary   = being->primary();
  Item* secondary = being->secondary();
  Item* weapon    = nullptr;

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
    being->room()->send_cond("$a parries $c's attack with $O!\n", being, _target_being, weapon, Room::TO_NOTVICT, true);
    // Should we riposte?
    being->invokeIfLearned(RIPOSTE, _target_being, weapon);
    return true;
  }

  return false;
}

bool RiposteSkill::execute(Being* being) const {
  std::string weapon_damage;

  if (_target_being == nullptr) {
    ERROR(being, "riposte without target being")
    return false;
  }
  if (_target_item == nullptr || !_target_item->isWeapon()) {
    ERROR(being, "riposte without target weapon")
    return false;
  }

  // 40% chance of riposte after parry
  if (Math::percent_chance(60)) return false;

  being->send("You quickly strike back at %s!\n", _target_being->name());
  _target_being->send("%s quickly strikes back at you!\n", being->name());
  being->room()->send_cond("$a quickly strikes back at $c!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
  Strike strike(being, _target_being, _target_item == being->primary());
  strike.nesting(1);
  strike.strike();

  return true;
}

bool DodgeSkill::execute(Being* being) const {
  if (_target_being == nullptr) {
    ERROR(being, "dodge without target")
    return false;
  }

  if (Math::percent_chance(40)) {
    being->send("You dodge %s's strike!\n", _target_being->name());
    _target_being->send("%s dodges your strike!\n", being->name());
    being->room()->send_cond("$a dodges $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
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
    being->send("You duck %s's strike!\n", _target_being->name());
    _target_being->send("%s ducks your strike!\n", being->name());
    being->room()->send_cond("$a ducks $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
    return true;
  }

  return false;
}

bool CounterattackSkill::execute(Being* being) const {
  if (_target_being == nullptr) {
    ERROR(being, "counterattack without target");
    return false;
  }

  if (Math::percent_chance(90)) return false;

  being->send("You counterattack %s!\n", _target_being->name());
  _target_being->send("%s counterattacks you!\n", being->name());
  being->room()->send_cond("$a counterattacks $c!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);
  Strike strike(being, _target_being, true);
  strike.nesting(1);
  strike.strike();

  return true;
}
