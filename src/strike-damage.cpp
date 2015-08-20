
#include <algorithm>
#include "strike-damage.hpp"
#include "being.hpp"
#include "item-types.hpp"
#include "os.hpp"

StrikeDamage::StrikeDamage(Being* attacker, Being* defender) {
  _attacker   = attacker;
  _defender   = defender;
  _base       = 0;
  _adjustment = 0;
  _defense    = 0;
  _offhand    = false;
  _unarmed    = false;
  _weapon     = nullptr;
  return;
}

StrikeDamage::~StrikeDamage(void) {
  return;
}

// Internal setup code.
void StrikeDamage::init(void) {
  Item* item = nullptr;

  item = offhand() ? _attacker->secondary() : _attacker->primary();
  if (item != nullptr && item->isWeapon()) {
    _weapon = item->weapon();
  } else {
    _unarmed = true;
  }

  return;
}

// Determine whether or not the attacker will land a hit.
bool StrikeDamage::hit(void) {
  int dex_diff = 0;

  // You miss less as your hit bonus increases.
  if (Math::rand(1, MAX(20, _attacker->hitBonus())) == 1) {
    return false;
  }

  // You miss less as your dexterity increases relative to your opponent.
  dex_diff = _attacker->dexterity() - _defender->dexterity();
  if (Math::rand(1, 50) < MAX(1, dex_diff)) {
    return false;
  }

  return true;
}

// Meta-method to calculate the amount of damage to be dealt.
unsigned StrikeDamage::getDamage(void) {
  int dam = 0;

  calculateBase();
  calculateAdjustments();
  calculateDefense();

  dam = _base + _adjustment - _defense;

  return MAX(1, dam);
}

// Calulate the baseline damage to be dealt.
void StrikeDamage::calculateBase(void) {
  if (_unarmed) {
    _base = _attacker->damBonus() + Math::rand(1, _attacker->strength());
  } else {
    _base = _weapon->damage().roll() + _attacker->damBonus();
    if (offhand()) _base *= 0.75;
  }
}

// Calculate an adjustment (as a percentage) to the baseline damage.
void StrikeDamage::calculateAdjustments(void) {
  unsigned stat_idx      = Being::ATTR_BEGIN;
  double stat_adjustment = 0.0;

  // Is this a critical strike? Chances increase with a higher hit bonus.
  if (Math::rand(1, 500) < _attacker->hitBonus()) {
    _adjustment += _base * CRIT_MULTIPLIER;
  }

  if (!_unarmed) {
    // Weapon key stat bonus.
    if ((stat_idx = _weapon->keyStat()) != Being::ATTR_UNDEFINED) {
      switch (stat_idx) {
        case Being::ATTR_HEALTH:  stat_adjustment = 1.0 * _attacker->health()   / _attacker->maxHealth(); break;
        case Being::ATTR_MANA:    stat_adjustment = 1.0 * _attacker->mana()     / _attacker->maxMana();   break;
        case Being::ATTR_STAMINA: stat_adjustment = 1.0 * _attacker->stamina()  / Being::MAX_STAMINA;     break;
        case Being::ATTR_STR:
        case Being::ATTR_DEX:
        case Being::ATTR_CON:
        case Being::ATTR_INT:
        case Being::ATTR_FOC:
        case Being::ATTR_CRE:
        case Being::ATTR_CHA:
        case Being::ATTR_LUC:
          stat_adjustment = 1.0 * _attacker->getAttribute(stat_idx) / Being::STAT_THRESHOLD;
          break;
        default:
          ERROR_(_attacker, "StrikeDamage::calculateAdjustments() bad stat_idx %u", stat_idx);
          break;
      }
      _adjustment += _base * stat_adjustment / 10;
    }

    // Weapon affinity.
    _adjustment += _base * (_attacker->affinity(!offhand()) / Being::AFFINITY_DEFAULT);
  }

  return;
}

// Calculate how much damage the defender can waive.
void StrikeDamage::calculateDefense(void) {

  _defense = 0; // ??

  return;
}

// Calculate the time (from now in seconds) when to schedule the next attack.
unsigned StrikeDamage::timeUntilNext(void) {
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
