
#include <algorithm>
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
  if (Math::rand(1, MAX(20, _attacker->hitBonus())) == 1) {
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
    _base = _attacker->damBonus() + Math::rand(1, _attacker->strength());
  } else {
    _base = _attacker->damBonus() + _weapon->damage().roll();
  }
}

void Attack::calculateAdjustments(void) {
  // Is this a critical strike? Chances increase with a higher hit bonus.
  if (Math::rand(1, 500) < _attacker->hitBonus()) {
    _adjustment += _base * CRIT_MULTIPLIER;
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
  if (!_unarmed) {
    modify += _attacker->strengthPercent() * _weapon->weightFactor();
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
