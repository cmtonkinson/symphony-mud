
#include "attack.hpp"
#include "being.hpp"
#include "item-types.hpp"

Attack::Attack(Being* attacker, Being* defender) {
  _attacker = attacker;
  _defender = defender;
  _base = 0;
  _adjustment = 0;
  _offhand = false;
  _unarmed = true;
  _weapon = nullptr;
  return;
}

Attack::~Attack(void) {
  return;
}

bool Attack::hit(void) {
  // You miss less as your hit bonus increases.
  // TODO - change 50 to a Being hit bonus stat
  if (Math::rand(1, 50) == 1) {
    return false;
  }

  // You miss less as your dexterity increases relative to your opponent.
  if (Math::rand(1, 50) < _attacker->dexterity() - _defender->dexterity()) {
    return false;
  }

  return true;
}

unsigned Attack::getDamage(void) {
  calculateBase();
  calculateAdjustments();
  return _base + _adjustment;
}

void Attack::calculateBase(void) {
  if (_unarmed) {
    // TODO - calculate base for openhanded strike, probably based on STR
    _base = 1;
  } else {
    _base = _weapon->damage().roll();
  }
}

void Attack::calculateAdjustments(void) {
  // Is this a critical strike? Chances increase with a higher damage bonus.
  // TODO - change 20 to Being damage bonus stat
  // TODO - change 0.4 to some CRIT_FACTOR const
  if (Math::rand(1, 500) < 20) {
    _adjustment += _base * 0.4;
  }

  // Weapon key stat bonus.
  // TODO - configurable "primary/key stat" for weapons
  _adjustment += _base * _attacker->strengthPercent();

  // Weapon affinity.
  // TODO - dynamic weapon affinity
  _adjustment += 0;

  return;
}

unsigned Attack::timeUntilNext(void) {
  double defalt = 2.0;
  double modify = 0.0;

  // You strike more frequently as dexterity increases.
  modify += defalt * (1.0 - (1.0 / _attacker->dexterityPercent()));

  // You strike more frequently as your strength increases relative to the
  // size/weight of your weapon.
  // TODO - replace 0 with strength factor
  // TODO - replace 1 with weapon factor (size vs compound(s) density)
  if (!_unarmed) {
    modify += 0 * 1;
  }

  return ROUND_2_UINT(defalt + modify);
}

void Attack::_init(void) {
  Item* item = nullptr;

  item = offhand() ? _attacker->secondary() : _attacker->primary();
  if (item != nullptr && item->isWeapon()) {
    _weapon = item->weapon();
  } else {
    _unarmed = true;
  }

  return;
}
