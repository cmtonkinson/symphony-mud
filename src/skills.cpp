
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
  return strike.strike();
}

bool ThirdStrikeSkill::execute(Being* being) const {
  if (Math::percent_chance(20)) return false;
  Strike strike(being, _target_being, true);
  return strike.strike();
}

bool FourthStrikeSkill::execute(Being* being) const {
  if (Math::percent_chance(20)) return false;
  Strike strike(being, _target_being, true);
  return strike.strike();
}

bool DualWieldSkill::execute(Being* being) const {
  Strike dual(being, _target_being, false);
  // Need an off-hand weapon to dual strike.
  if (dual.unarmed()) return false;
  // Moderate chance of success.
  if (Math::percent_chance(30)) return false;
  // Go for it.
  return dual.strike();
}

///////////////////////////////////////////////////////////////////////////////
// AVOIDANCE
///////////////////////////////////////////////////////////////////////////////
bool BlockSkill::execute(Being* being) const {
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "block without target")
    return false;
  }

  if (Math::percent_chance(40)) return false;

  _target_being->currentStrike()->setStatus(Strike::BLOCKED);

  spaces = _target_being->currentStrike()->sequenceNumber() * 2;
  being->indentedSend(spaces, "You block %s's strike!\n", _target_being->name());
  _target_being->indentedSend(spaces, "%s blocks your strike!\n", being->name());
  being->room()->indented_send_cond(spaces, "$a blocks $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);

  return true;
}

bool DodgeSkill::execute(Being* being) const {
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "dodge without target")
    return false;
  }

  if (Math::percent_chance(60)) return false;

  _target_being->currentStrike()->setStatus(Strike::EVADED);

  spaces = _target_being->currentStrike()->sequenceNumber() * 2;
  being->indentedSend(spaces, "You dodge %s's strike!\n", _target_being->name());
  _target_being->indentedSend(spaces, "%s dodges your strike!\n", being->name());
  being->room()->indented_send_cond(spaces, "$a dodges $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);

  return true;
}

bool DuckSkill::execute(Being* being) const {
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "duck without target")
    return false;
  }

  if (Math::percent_chance(60)) return false;

  _target_being->currentStrike()->setStatus(Strike::EVADED);

  spaces = _target_being->currentStrike()->sequenceNumber() * 2;
  being->indentedSend(spaces, "You duck %s's strike!\n", _target_being->name());
  _target_being->indentedSend(spaces, "%s ducks your strike!\n", being->name());
  being->room()->indented_send_cond(spaces, "$a ducks $c's strike!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);

  return true;
}

bool ParrySkill::execute(Being* being) const {
  Item* primary   = being->primary();
  Item* secondary = being->secondary();
  Item* weapon    = nullptr;
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "parry without target")
    return false;
  }

  if (primary == nullptr && secondary == nullptr) {
    ERROR(being, "parry without weapon")
    return false;
  }

  if (primary != nullptr && secondary != nullptr) {
    weapon = Math::percent_chance(60) ? primary : secondary;
  } else {
    weapon = primary != nullptr ? primary : secondary;
  }

  if (!weapon->isWeapon()) return false;

  if (Math::percent_chance(40)) return false;

  _target_being->currentStrike()->setStatus(Strike::BLOCKED);

  spaces = _target_being->currentStrike()->sequenceNumber() * 2;
  being->indentedSend(spaces, "You parry %s's attack with %s!\n", _target_being->name(), weapon->name());
  _target_being->indentedSend(spaces, "%s parries your attack with %s!\n", being->name(), weapon->name());
  being->room()->indented_send_cond(spaces, "$a parries $c's attack with $O!\n", being, _target_being, weapon, Room::TO_NOTVICT, true);
  // Should we riposte?
  being->invokeIfLearned(RIPOSTE, _target_being, weapon);

  return true;
}

bool RiposteSkill::execute(Being* being) const {
  std::string weapon_damage;
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "riposte without target being")
    return false;
  }
  if (_target_item == nullptr || !_target_item->isWeapon()) {
    ERROR(being, "riposte without target weapon")
    return false;
  }

  if (Math::percent_chance(60)) return false;

  spaces = (_target_being->currentStrike()->sequenceNumber() + 1) * 2;
  being->indentedSend(spaces, "You quickly strike back at %s!\n", _target_being->name());
  _target_being->indentedSend(spaces, "%s quickly strikes back at you!\n", being->name());
  being->room()->indented_send_cond(spaces, "$a quickly strikes back at $c!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);

  Strike strike(being, _target_being, _target_item == being->primary());
  strike.reactTo(_target_being->currentStrike());

  return true;
}

bool CounterattackSkill::execute(Being* being) const {
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "counterattack without target");
    return false;
  }

  if (Math::percent_chance(95)) return false;

  _target_being->currentStrike()->setStatus(Strike::COUNTERED);

  spaces = (_target_being->currentStrike()->sequenceNumber() + 1) * 2;
  being->indentedSend(spaces, "You counterattack %s!\n", _target_being->name());
  _target_being->indentedSend(spaces, "%s counterattacks you!\n", being->name());
  being->room()->indented_send_cond(spaces, "$a counterattacks $c!\n", being, _target_being, nullptr, Room::TO_NOTVICT, true);

  Strike strike(being, _target_being, true);
  strike.reactTo(_target_being->currentStrike());

  return true;
}

bool ShieldBlockSkill::execute(Being* being) const {
  Item* shield = being->shield();
  unsigned spaces = 0;

  if (_target_being == nullptr) {
    ERROR(being, "shield block without target")
    return false;
  }

  if (shield == nullptr || !shield->isArmor()) {
    ERROR(being, "shield block without shield")
    return false;
  }

  if (Math::percent_chance(40)) return false;

  _target_being->currentStrike()->setStatus(Strike::BLOCKED);

  spaces = _target_being->currentStrike()->sequenceNumber() * 2;
  being->indentedSend(spaces, "You block %s's strike with %s!\n", _target_being->name(), shield->name());
  _target_being->indentedSend(spaces, "%s blocks your strike with %s!\n", being->name(), shield->name());
  being->room()->indented_send_cond(spaces, "$a blocks $c's strike with $O!\n", being, _target_being, shield, Room::TO_NOTVICT, true);

  return true;
}
