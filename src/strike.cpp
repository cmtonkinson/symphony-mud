
#include "ability.hpp"
#include "being.hpp"
#include "display.hpp"
#include "item-types.hpp"
#include "os.hpp"
#include "skills.hpp"
#include "strike.hpp"

Strike::Strike(void):
    _attacker(nullptr),
    _defender(nullptr),
    _weapon(nullptr),
    _primary(true),
    _unarmed(true),
    _status(0),
    _sequence_number(0),
    _base(0),
    _adjustment(0),
    _defense(0) {
  _init();
  return;
}

Strike::Strike(Being* attacker_, Being* defender_, bool primary): Strike() {
  attacker(attacker_);
  defender(defender_);
  _init();
  return;
}

Strike::Strike(const Strike& ref) {
  *this = ref;
  return;
}

Strike::~Strike(void) {
  return;
}

Strike& Strike::operator = (const Strike& ref) {
  _attacker        = ref.attacker();
  _defender        = ref.defender();
  _weapon          = ref.weapon();
  _primary         = ref.primary();
  _unarmed         = ref.unarmed();
  _status          = ref.status();
  _sequence_number = ref.sequenceNumber();
  _base            = ref.base();
  _adjustment      = ref.adjustment();
  _defense         = ref.defense();
  return *this;
}

void Strike::weapon(Item* weapon) {
  if (weapon != nullptr && weapon->isWeapon()) _weapon = weapon->weapon();
  return;
}

// Determine whether or not the attacker will land a hit.
bool Strike::willMiss(void) {
  double dex_diff = 0.0;

  // You miss less as your hit bonus increases.
  if (Math::rand(1, MAX(20, _attacker->hitBonus())) == 1) {
    return true;
  }

  // You miss less as your dexterity increases relative to your opponent.
  dex_diff = _attacker->dexterity() / _defender->dexterity();
  if (Math::percent_chance(pow(2.0, dex_diff))) {
    return true;
  }

  return false;
}

// Meta-method to calculate the amount of damage to be dealt.
unsigned Strike::getDamage(void) {
  int dam = 0;

  calculateBase();
  calculateAdjustments();
  calculateDefense();

  dam = _base + _adjustment - _defense;

  return MAX(1, dam);
}

// Calulate the baseline damage to be dealt.
void Strike::calculateBase(void) {
  if (_unarmed) {
    _base = (_attacker->level() / 2) + Math::rand(1, _attacker->strength()) + _attacker->damBonus();
  } else {
    _base = _weapon->damage().roll() + _attacker->damBonus();
    if (!primary()) _base *= OFFHAND_PENALTY;
  }
}

// Calculate an adjustment (as a percentage) to the baseline damage.
void Strike::calculateAdjustments(void) {
  unsigned stat_idx      = Being::ATTR_BEGIN;
  double stat_adjustment = 0.0;

  // TODO - lose some effectiveness with low health
  // TODO - increased crit chance with very low health

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
          ERROR_(_attacker, "Strike::calculateAdjustments() bad stat_idx %u", stat_idx);
          break;
      }
      _adjustment += _base * stat_adjustment / 10;
    }

    // Weapon affinity.
    _adjustment += _base * (_attacker->affinity(primary()) / Being::AFFINITY_DEFAULT);
  }

  return;
}

// Calculate how much damage the defender can waive.
void Strike::calculateDefense(void) {
  int type_defense = 0;

  // Start from scratch.
  _defense = 0;

  // Higher constitution will significantly lower damage taken.
  _defense += _base * _defender->constitutionPercent() / 5;

  // Dexterity helps you to absorb the blow - so it comes with a bonus.
  _defense += _base * _defender->dexterityPercent() / 10;

  // Armor has a direct correlation to damage avoided.
  _defense += _defender->armor() / 8;

  // Specific armor classes have a greater correlation to damage avoided.
  if (!_unarmed) {
    switch (ItemWeapon::damageType(_weapon)) {
      case ItemWeapon::DAMAGE_BASH:   type_defense += _defender->bash();    break;
      case ItemWeapon::DAMAGE_SLASH:  type_defense += _defender->slash();   break;
      case ItemWeapon::DAMAGE_PIERCE: type_defense += _defender->pierce();  break;
      case ItemWeapon::DAMAGE_EXOTIC: type_defense += _defender->exotic();  break;
    }
    _defense += type_defense / 4;
  }

  // armor of specific item hit
  // TODO - item durability

  return;
}

bool Strike::strike(void) {
  int damage = 0;
  unsigned spaces = sequenceNumber() * 2;
  std::string weapon_string;
  std::string damage_string;

  // Zombie warfare? No thanks, guy.
  if (_attacker->isDead() || _defender->isDead()) return false;

  // What is the correct verb for the strike?
  weapon_string = _unarmed ? "strike" : _weapon->verb().string();

  // Prevent infinite combat sequences (by bug or by chance).
  if (_sequence_number > SEQUENCE_LIMIT) return false;
  // Check for stamina.
  if (!_attacker->deplete_stamina(1)) return false;

  _attacker->currentStrike(this);

  // Will we miss?
  if (willMiss()) {
    _attacker->deplete_stamina(1);
    _status &= MISSED;
    _attacker->indentedSend(spaces, "Your %s misses %s!\n", weapon_string.c_str(), _defender->name());
    _defender->indentedSend(spaces, "%s's %s misses you!\n", _attacker->name(), weapon_string.c_str());
    _defender->room()->indented_send_cond(spaces, "$a's $s misses $C!", _attacker, weapon_string.c_str(), _defender, Room::TO_NOTVICT);
  // Will they avoid the strike somehow?
  } else if (avoid()) {
    _status &= AVOIDED;
  // What's the damage?
  } else {
    damage = getDamage();
    damage_string = weapon_string.append(" ").append(Display::formatDamage(damage));
    _status &= HIT;
    _attacker->indentedSend(spaces, "Your %s %s!\n", damage_string.c_str(), _defender->name());
    _defender->indentedSend(spaces, "%s's %s you!\n", _attacker->name(), damage_string.c_str());
    _defender->room()->indented_send_cond(spaces, "$a's $s $C!", _attacker, (void*)damage_string.c_str(), _defender, Room::TO_NOTVICT, true);
    _defender->takeDamage(damage, _attacker);

    // Is there a counter-offensive?
    _defender->invokeIfLearned(COUNTERATTACK, _attacker);
  }

  _attacker->currentStrike(nullptr);
  _attacker->lastStrike(*this);
  return true;
}

// TODO - refactor such that the probability of invoking an avoidance skill increases proportionately
// to the number of avoidance skills. Current algorithm provides zero incentive to learn more than one
// such skill.
bool Strike::avoid(void) {
  Ability* skill = nullptr;
  std::vector<Ability*> evasion_skills;

  if (Math::percent_chance(60)) return false;

  // What evasion methods are available?
  if ((skill = _defender->learned().find_skill(BLOCK)) != nullptr) evasion_skills.push_back(skill);
  if ((skill = _defender->learned().find_skill(DODGE)) != nullptr) evasion_skills.push_back(skill);
  if ((skill = _defender->learned().find_skill(DUCK)) != nullptr) evasion_skills.push_back(skill);
  if (_defender->shield()) {
    if ((skill = _defender->learned().find_skill(SHIELD_BLOCK)) != nullptr) evasion_skills.push_back(skill);
  }
  if (_defender->primary() || _defender->secondary()) {
    if ((skill = _defender->learned().find_skill(PARRY)) != nullptr) evasion_skills.push_back(skill);
  }
  if (evasion_skills.empty()) return false;

  // Select an evasion Skill.
  skill = evasion_skills[Math::rand(0, evasion_skills.size() - 1)];
  return skill->invoke(_defender, _attacker);
}

bool Strike::reactTo(const Strike* antecedent) {
  _sequence_number = antecedent->sequenceNumber() + 1;
  return strike();
}

// Calculate the time (from now in seconds) when to schedule the next attack.
unsigned Strike::timeUntilNext(void) {
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

// private
void Strike::_init(void) {
  if (_attacker == nullptr) return;
  weapon(primary() ? _attacker->primary() : _attacker->secondary());
  unarmed(weapon() == nullptr);
  return;
}
