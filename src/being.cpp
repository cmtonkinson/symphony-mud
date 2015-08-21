
#include <vector>
#include "ability.hpp"
#include "avatar.hpp"
#include "being.hpp"
#include "command-def.hpp"
#include "display.hpp"
#include "exit.hpp"
#include "group.hpp"
#include "identifiers.hpp"
#include "io-handler.hpp"
#include "item-types.hpp"
#include "job.hpp"
#include "npc.hpp"
#include "os.hpp"
#include "room.hpp"
#include "stats.hpp"
#include "world.hpp"
#include "zone.hpp"

Being::Being(void):
    _inventory(&Identifiers::shortname) {
  ID(0);
  room(NULL);
  position().set(STANDING);
  action().set(0);
  formGroup();
  // identity...
  gender().set(NEUTRAL);
  race().set(0);
  pClass().set(0);
  // stats...
  furniture(NULL);
  // Level
  level(1);
  exp(BASE_EXP);
  tnl(BASE_TNL);
  // Health
  maxHealth(BASE_HEALTH);
  health(BASE_HEALTH);
  maxMana(BASE_MANA);
  mana(BASE_MANA);
  stamina(MAX_STAMINA);
  // Core Stats
  strength(STAT_BASE);
  dexterity(STAT_BASE);
  constitution(STAT_BASE);
  intelligence(STAT_BASE);
  focus(STAT_BASE);
  creativity(STAT_BASE);
  charisma(STAT_BASE);
  luck(STAT_BASE);
  // Extended Stats
  hitBonus(0);
  damBonus(0);
  sizeAffinity(AFFINITY_DEFAULT);
  rangeAffinity(AFFINITY_DEFAULT);
  // Armor
  armor(100);
  bash(0);
  slash(0);
  pierce(0);
  exotic(0);
  // Misc
  trains(BASE_TRAINS);
  money(0);
  // combat
  _next_attack = NULL;
  _target      = NULL;
  return;
}

Being::Being(const Being& ref):
    _identifiers(ref.identifiers()),
    _inventory(ref.inventory()),
    _equipment(ref.equipment()),
    _position(ref.position()),
    _action(ref.action()),
    // identity...
    _gender(ref.gender()),
    _race(ref.race()),
    _pClass(ref.pClass()) {
  ID(ref.ID());
  level(ref.level());
  room(NULL);
  for (std::list<Modifier*>::const_iterator it = ref.modifiers().begin(); it != ref.modifiers().end(); ++it) {
    modifiers().push_back(new Modifier(**it));
  }
  furniture(ref.furniture());
  formGroup();
  // stats...
  level(ref.level());
  exp(ref.exp());
  tnl(ref.tnl());
  maxHealth(ref.maxHealth());
  health(ref.health());
  maxMana(ref.maxMana());
  mana(ref.mana());
  stamina(ref.stamina());
  strength(ref.strength());
  dexterity(ref.dexterity());
  constitution(ref.constitution());
  intelligence(ref.intelligence());
  focus(ref.focus());
  creativity(ref.creativity());
  charisma(ref.charisma());
  luck(ref.luck());
  hitBonus(ref.hitBonus());
  damBonus(ref.damBonus());
  sizeAffinity(ref.sizeAffinity());
  rangeAffinity(ref.rangeAffinity());
  armor(ref.armor());
  bash(ref.bash());
  slash(ref.slash());
  pierce(ref.pierce());
  exotic(ref.exotic());
  trains(ref.trains());
  money(ref.money());
  // combat...
  _next_attack = NULL;
  _target      = NULL;
  return;
}

Being::~Being(void) {
  for (std::vector<IOHandler*>::iterator it = IOhandlers().begin(); IOhandlers().size();) {
    delete *it;
    it = IOhandlers().erase(it);
  }
  // Remove the Being from its group.
  ungroup();
  // Remove any Jobs set on this Being.
  World::Instance().schedule()->cleanup(this);
  return;
}

IOHandler* Being::IOhandler(void) {
  return IOhandlers().back();
}

void Being::pushIOHandler(IOHandler* handler) {
  IOhandlers().push_back(handler);
  handler->activate();
  return;
}

void Being::popIOHandler(void) {
  IOhandlers().back()->deactivate();
  IOhandlers().pop_back();
  return;
}

void Being::replaceIOHandler(IOHandler* handler) {
  popIOHandler();
  pushIOHandler(handler);
  return;
}

void Being::handle(void) {
  IOhandlers().back()->handle();
  return;
}

void Being::animate(void) {
  // TODO - vary auto_heal period based on... something
  World::Instance().schedule()->add(new RecurringJob(this, &Being::auto_health,   "Being::auto_health", 50, 70));
  World::Instance().schedule()->add(new RecurringJob(this, &Being::auto_mana,     "Being::auto_mana", 50, 70));
  World::Instance().schedule()->add(new RecurringJob(this, &Being::auto_stamina,  "Being::auto_stamina", 2, 4));
  _animate();
  return;
}

Klass* Being::klass(void) const {
  switch (pClass().number()) {
    case CLERIC:  return &Cleric::Instance();
    case MAGE:    return &Mage::Instance();
    case ROGUE:   return &Rogue::Instance();
    case WARRIOR: return &Warrior::Instance();
    default:      return NULL;
  }
}

void Being::naturalStatAdjustment(void) {
  switch (race().number()) {
    case ELF:
      _dexterity    += 2;
      _intelligence += 1;
      _charisma     += 2;
      _constitution -= 1;
      break;
    case DWARF:
      _strength     += 2;
      _constitution += 2;
      _luck         += 3;
      _dexterity    -= 1;
      _charisma     -= 1;
      break;
    case HUMAN:
      break;
  }
  switch (pClass().number()) {
    case CLERIC:
      _focus        += 2;
      _creativity   -= 1;
      _strength     -= 1;
      break;
    case MAGE:
      _intelligence += 2;
      _focus        += 2;
      _creativity   += 2;
      _strength     -= 2;
      _constitution -= 2;
      break;
    case ROGUE:
      _dexterity    += 2;
      _intelligence += 1;
      _creativity   += 1;
      _constitution -= 1;
      _charisma     -= 2;
      break;
    case WARRIOR:
      _strength     += 2;
      _dexterity    += 1;
      _constitution += 2;
      _intelligence -= 2;
      _focus        -= 2;
      break;
  }
  return;
}

// Never set health above the max.
void Being::health(int health) {
  _health = MIN(health, maxHealth());
  return;
}

// When altering maxHealth, health should translate appropriately.
void Being::maxHealth(int maxHealth) {
  if (maxHealth > _maxHealth) {
    int diff = maxHealth - _maxHealth;
    _maxHealth = maxHealth;
    _health += diff;
  } else if (maxHealth < _maxHealth) {
    _maxHealth = maxHealth;
    _health = MIN(_health, maxHealth);
  }
  return;
}

void Being::mana(int mana) {
  _mana = MIN(mana, maxMana());
}

void Being::maxMana(int maxMana) {
  _maxMana = maxMana;
}

void Being::stamina(unsigned stamina) {
  _stamina = MIN(stamina, MAX_STAMINA);
  return;
}

std::string Being::serializeAbilities(void) {
  std::vector<std::string> foo;
  char buf[128];
  for (auto iter : abilityMastery()) {
    sprintf(buf, "%s:%u", iter.first->name().c_str(), iter.second);
    foo.push_back(buf);
  }
  return Regex::implode("~", foo);
}

void Being::unserializeAbilities(std::string ser) {
  Ability* ability = nullptr;
  unsigned mastery = 0;
  std::vector<std::string> foo = Regex::explode("~", ser);
  std::vector<std::string> bar;
  for (auto iter : foo) {
    bar = Regex::explode(":", iter);
    if ((ability = klass()->abilities().find(bar[0])) == NULL) continue;
    sscanf(bar[1].c_str(), "%u", &mastery);
    learn(ability, mastery);
  }
  return;
}

Being* Being::findBeing(const std::string& name) {
  if (name == "self") return this;

  for (std::list<Being*>::iterator it = room()->beings().begin(); it != room()->beings().end(); ++it) {
    if ((*it)->identifiers().matchesKeyword(name) && canSee(*it) == Being::SEE_NAME) {
      return *it;
    }
  }
  return NULL;
}

Item* Being::findItem(const std::string& query) {
  Item* i = NULL;
  if ((i = inventory().searchSingleItem(query)) == NULL) {
    if ((i = equipment().searchSingleItem(query)) == NULL) {
      if ((i = room()->inventory().searchSingleItem(query)) == NULL) {
        return NULL;
      }
    }
  }
  return i;
}

unsigned short Being::getWearloc(const Item::Wearable& wearable) const {
  switch (wearable) {
    case Item::Wearable_Head:       return WEARLOC_HEAD;
    case Item::Wearable_Ear:        return WEARLOC_EAR_L;
    case Item::Wearable_Face:       return WEARLOC_FACE;
    case Item::Wearable_Neck:       return WEARLOC_NECK;
    case Item::Wearable_Shoulders:  return WEARLOC_SHOULDERS;
    case Item::Wearable_Arms:       return WEARLOC_ARMS;
    case Item::Wearable_Torso:      return WEARLOC_TORSO;
    case Item::Wearable_Forearm:    return WEARLOC_FOREARM_L;
    case Item::Wearable_Wrist:      return WEARLOC_WRIST_L;
    case Item::Wearable_Hands:      return WEARLOC_HANDS;
    case Item::Wearable_Hold:       return WEARLOC_HOLD_L;
    case Item::Wearable_Finger:     return WEARLOC_FINGER_L;
    case Item::Wearable_Waist:      return WEARLOC_WAIST_1;
    case Item::Wearable_Legs:       return WEARLOC_LEGS;
    case Item::Wearable_Knee:       return WEARLOC_KNEE_L;
    case Item::Wearable_Shin:       return WEARLOC_SHIN_L;
    case Item::Wearable_Ankle:      return WEARLOC_ANKLE_L;
    case Item::Wearable_Feet:       return WEARLOC_FEET;
    default:                          return WEARLOC_END;
  }
}

const char* Being::wearLocName(const unsigned short& wearloc) {
  switch (wearloc) {
    case WEARLOC_HEAD:      return "<{ghead{x>          ";
    case WEARLOC_EAR_L:     return "<{gleft ear{x>      ";
    case WEARLOC_EAR_R:     return "<{gright ear{x>     ";
    case WEARLOC_FACE:      return "<{gface{x>          ";
    case WEARLOC_NECK:      return "<{gneck{x>          ";
    case WEARLOC_SHOULDERS: return "<{gshoulders{x>     ";
    case WEARLOC_ARMS:      return "<{garms{x>          ";
    case WEARLOC_TORSO:     return "<{gtorso{x>         ";
    case WEARLOC_FOREARM_L: return "<{gleft forearm{x>  ";
    case WEARLOC_FOREARM_R: return "<{gright forearm{x> ";
    case WEARLOC_WRIST_L:   return "<{gleft wrist{x>    ";
    case WEARLOC_WRIST_R:   return "<{gright wrist{x>   ";
    case WEARLOC_HANDS:     return "<{ghands{x>         ";
    case WEARLOC_HOLD_L:    return "<{gleft hand{x>     ";
    case WEARLOC_HOLD_R:    return "<{gright hand{x>    ";
    case WEARLOC_FINGER_L:  return "<{gleft finger{x>   ";
    case WEARLOC_FINGER_R:  return "<{gright finger{x>  ";
    case WEARLOC_WAIST_1:   return "<{gwaist{x>         ";
    case WEARLOC_WAIST_2:   return "<{gwaist{x>         ";
    case WEARLOC_LEGS:      return "<{glegs{x>          ";
    case WEARLOC_KNEE_L:    return "<{gleft knee{x>     ";
    case WEARLOC_KNEE_R:    return "<{gright knee{x>    ";
    case WEARLOC_SHIN_L:    return "<{gleft shin{x>     ";
    case WEARLOC_SHIN_R:    return "<{gright shin{x>    ";
    case WEARLOC_ANKLE_L:   return "<{gleft ankle{x>    ";
    case WEARLOC_ANKLE_R:   return "<{gright ankle{x>   ";
    case WEARLOC_FEET:      return "<{gfeet{x>          ";
    default:                return "[{Rerror{x]       ";
  }
}

bool Being::wear(Item* article, std::string& message, Item*& removed) {
  int location = getWearloc(article->wearable());

  // make sure we can wear it...
  if (location == WEARLOC_ERROR) {
    message.assign("You can't wear ").append(article->identifiers().shortname()).append("{x.");
    return false;
  } else if (article->level() > level()) {
    message.assign("You're not powerful enough to wear ").append(article->identifiers().shortname()).append("{x yet.");
    return false;
  }

  // for single-slot items (head, float, etc)
  if (isSingleWearLoc(article->wearable())) {
    if (worn(location)) {
      if (!unwear(worn(location), message)) {
        return false;
      }
    }
  // edge case to ensure that weapons are held in the primary hand by default
  // duplicate of the other "double-slot" items below
  } else if (location == WEARLOC_HOLD_L || location == WEARLOC_HOLD_R) {
    if (worn(hand()) == NULL) {
      location = hand();
    } else if (worn(off_hand()) == NULL) {
      location = off_hand();
    } else if ((unwear(removed = worn(hand()), message))) {
      location = hand();
    } else if ((unwear(removed = worn(off_hand()), message))) {
      location = off_hand();
    }
  // for other double-slot items (like ears, fingers, etc)
  } else {
    // check the first slot
    if (worn(location) == NULL) { }
    // check the second slot
    else if (worn(location+1) == NULL) {
      ++location;
    }
    // try to force the first slot
    else if ((unwear(removed = worn(location), message))) { }
    // try to force the second slot
    else if ((unwear(removed = worn(location+1), message))) {
      ++location;
    }
    // both slots are taken and can't be reclaimed at this time
    else {
      return false;
    }
  }

  // don the article...
  inventory().remove(article);
  equipment().add(article, location);
  setModifications(article);
  return true;
}

bool Being::unwear(Item* article, std::string& message, bool force) {
  if (!force) {
    if (article->flags().test(ITEM_NOREMOVE)) {
      message.assign("You can't remove ").append(article->identifiers().shortname().c_str()).append("{x.");
      return false;
    }
  }
  for (std::map<int,Item*>::iterator it = equipment().itemMap().begin(); it != equipment().itemMap().end(); ++it) {
    if (it->second == article) {
      equipment().remove(article);
      inventory().add(article);
      unsetModifications(article);
      return true;
    }
  }
  message.assign("can't find it");
  return false;
}

Item* Being::worn(const int& location) const {
  std::map<int,Item*>::const_iterator it;
  if ((it = equipment().itemMap().find(location)) != equipment().itemMap().end()) {
    return it->second;
  }
  return NULL;
}

bool Being::isSingleWearLoc(const unsigned short& item_weartype) {
  switch (item_weartype) {
    case Item::Wearable_Head:
    case Item::Wearable_Face:
    case Item::Wearable_Neck:
    case Item::Wearable_Shoulders:
    case Item::Wearable_Arms:
    case Item::Wearable_Torso:
    case Item::Wearable_Hands:
    case Item::Wearable_Legs:
    case Item::Wearable_Feet:
      return true;
    default:
      return false;
  }
}

Item* Being::primary(void) {
  return equipment().at(hand());
}

Item* Being::secondary(void) {
  return equipment().at(off_hand());
}

bool Being::lay(std::string& error, ItemFurniture* furniture) {
  if (isLaying()) {
    error.assign("You're already laying down.");
    return false;
  }
  if (!isSitting() && !isStanding()) {
    error.assign("You can't lie down while ").append(position().string()).append(".");
    return false;
  }
  if (furniture) {
    if (furniture->add(this, FURN_LAY_ON)) {
      this->furniture(furniture);
      position().set(LAYING);
      return true;
    } else {
      error.assign("You can't lay on that.");
      return false;
    }
  }
  position().set(LAYING);
  return true;
}

bool Being::sit(std::string& error, ItemFurniture* furniture, bool on) {
  EnumInt pos = on? FURN_SIT_ON : FURN_SIT_AT;
  if (isSitting()) {
    error.assign("You're already sitting.");
    return false;
  }
  if (!isLaying() && !isStanding()) {
    error.assign("You can't sit while ").append(position().string()).append(".");
    return false;
  }
  if (furniture) {
    if (furniture->add(this, pos)) {
      this->furniture(furniture);
      position().set(SITTING);
      return true;
    } else {
      if (on) {
        error.assign("You can't sit on that.");
      } else {
        error.assign("You can't sit there.");
      }
      return false;
    }
  }
  position().set(SITTING);
  return true;
}

bool Being::stand(std::string& error) {
  if (isStanding()) {
    error.assign("You're already standing.");
    return false;
  }
  if (isMounted()) {
    error.assign("You can't do that while ").append(position().string()).append(".");
    return false;
  }
  if (furniture()) {
    furniture()->remove(this);
    furniture(NULL);
  }
  position().set(STANDING);
  return true;
}

unsigned short Being::stringToAttribute(const std::string& name) {
  if (Regex::strPrefix(name, "health")) {
    return ATTR_HEALTH;
  } else if (Regex::strPrefix(name, "max health")) {
    return ATTR_MAX_HEALTH;
  } else if (Regex::strPrefix(name, "mana")) {
    return ATTR_MANA;
  } else if (Regex::strPrefix(name, "max mana")) {
    return ATTR_MAX_MANA;
  } else if (Regex::strPrefix(name, "stamina")) {
    return ATTR_STAMINA;
  } else if (Regex::strPrefix(name, "str")) {
    return ATTR_STR;
  } else if (Regex::strPrefix(name, "dex")) {
    return ATTR_DEX;
  } else if (Regex::strPrefix(name, "con")) {
    return ATTR_CON;
  } else if (Regex::strPrefix(name, "int")) {
    return ATTR_INT;
  } else if (Regex::strPrefix(name, "foc")) {
    return ATTR_FOC;
  } else if (Regex::strPrefix(name, "cre")) {
    return ATTR_CRE;
  } else if (Regex::strPrefix(name, "cha")) {
    return ATTR_CHA;
  } else if (Regex::strPrefix(name, "luc")) {
    return ATTR_LUC;
  } else if (Regex::strPrefix(name, "armor")) {
    return ATTR_ARMOR;
  } else if (Regex::strPrefix(name, "bash")) {
    return ATTR_BASH;
  } else if (Regex::strPrefix(name, "slash")) {
    return ATTR_SLASH;
  } else if (Regex::strPrefix(name, "pierce")) {
    return ATTR_PIERCE;
  } else if (Regex::strPrefix(name, "exotic")) {
    return ATTR_EXOTIC;
  } else if (Regex::strPrefix(name, "hit")) {
    return ATTR_HIT;
  } else if (Regex::strPrefix(name, "dam")) {
    return ATTR_DAM;
  } else {
    return ATTR_END;
  }
}

const char* Being::attributeToString(const unsigned short& index) {
  switch (index) {
    case ATTR_HEALTH:     return "health";
    case ATTR_MAX_HEALTH: return "max health";
    case ATTR_MANA:       return "mana";
    case ATTR_MAX_MANA:   return "max mana";
    case ATTR_STAMINA:    return "stamina";
    case ATTR_STR:        return "str";
    case ATTR_DEX:        return "dex";
    case ATTR_CON:        return "con";
    case ATTR_INT:        return "int";
    case ATTR_FOC:        return "foc";
    case ATTR_CRE:        return "cre";
    case ATTR_CHA:        return "cha";
    case ATTR_LUC:        return "luc";
    case ATTR_ARMOR:      return "armor";
    case ATTR_BASH:       return "bash";
    case ATTR_SLASH:      return "slash";
    case ATTR_PIERCE:     return "pierce";
    case ATTR_EXOTIC:     return "exotic";
    case ATTR_HIT:        return "hit";
    case ATTR_DAM:        return "dam";
    default:              return "[error]";
  }
}

std::string Being::listAttributes(void) {
  std::string output;
  for (unsigned i = ATTR_BEGIN; i < ATTR_END; ++i) {
    output.append(attributeToString(i)).append(1, ' ');
  }
  if (!output.empty()) {
    output.resize(output.size()-1);
  }
  return output;
}

std::string Being::ident(void) const {
  std::string dest = identifiers().shortname();
  if (isAvatar()) return dest << std::string(" @") << toAvatar()->ip();
  else if (isNpc()) return dest << std::string(" #") << toNpc()->vnum();
  else return "error";
}

void Being::setModifications(Item* item) {
  for (std::list<Modifier*>::const_iterator it = item->modifiers().begin(); it != item->modifiers().end(); ++it) {
    modify(*it);
  }
  return;
}

void Being::unsetModifications(Item* item) {
  for (std::list<Modifier*>::const_iterator it = item->modifiers().begin(); it != item->modifiers().end(); ++it) {
    unmodify(*it);
  }
  return;
}

void Being::modify(Modifier* modifier) {
  doModification(modifier->attribute(), modifier->magnitude());
  modifiers().push_back(modifier);
  return;
}

void Being::unmodify(Modifier* modifier) {
  doModification(modifier->attribute(), -(modifier->magnitude()));
  modifiers().remove(modifier);
  return;
}

void Being::doModification(const unsigned short& attribute, const int& magnitude) {
  switch (attribute) {
    case ATTR_HEALTH:     health(      health()       + magnitude); break;
    case ATTR_MAX_HEALTH: maxHealth(   maxHealth()    + magnitude); break;
    case ATTR_MANA:       mana(        mana()         + magnitude); break;
    case ATTR_MAX_MANA:   maxMana(     maxMana()      + magnitude); break;
    case ATTR_STAMINA:    stamina(     stamina()      + magnitude); break;
    case ATTR_STR:        strength(    strength()     + magnitude); break;
    case ATTR_DEX:        dexterity(   dexterity()    + magnitude); break;
    case ATTR_CON:        constitution(constitution() + magnitude); break;
    case ATTR_INT:        intelligence(intelligence() + magnitude); break;
    case ATTR_FOC:        focus(       focus()        + magnitude); break;
    case ATTR_CRE:        creativity(  creativity()   + magnitude); break;
    case ATTR_CHA:        charisma(    charisma()     + magnitude); break;
    case ATTR_LUC:        luck(        luck()         + magnitude); break;
    case ATTR_ARMOR:      armor(       armor()        + magnitude); break;
    case ATTR_BASH:       bash(        bash()         + magnitude); break;
    case ATTR_SLASH:      slash(       slash()        + magnitude); break;
    case ATTR_PIERCE:     pierce(      pierce()       + magnitude); break;
    case ATTR_EXOTIC:     exotic(      exotic()       + magnitude); break;
    case ATTR_HIT:        hitBonus(    hitBonus( )    + magnitude); break;
    case ATTR_DAM:        damBonus(    damBonus( )    + magnitude); break;
    default: break;
  }
  return;
}

int Being::getAttribute(unsigned attribute) {
  switch (attribute) {
    case ATTR_MAX_HEALTH:   return maxHealth();
    case ATTR_HEALTH:       return health();
    case ATTR_MAX_MANA:     return maxMana();
    case ATTR_MANA:         return mana();
    case ATTR_STAMINA:      return stamina();
    case ATTR_STR:          return strength();
    case ATTR_DEX:          return dexterity();
    case ATTR_CON:          return constitution();
    case ATTR_INT:          return intelligence();
    case ATTR_FOC:          return focus();
    case ATTR_CRE:          return creativity();
    case ATTR_CHA:          return charisma();
    case ATTR_LUC:          return luck();
    case ATTR_ARMOR:        return armor();
    case ATTR_BASH:         return bash();
    case ATTR_SLASH:        return slash();
    case ATTR_PIERCE:       return pierce();
    case ATTR_EXOTIC:       return exotic();
    case ATTR_HIT:          return hitBonus();
    case ATTR_DAM:          return damBonus();
    default:
      ERROR_(this, "getAttribute() passed bad index: %u", attribute);
      return 0;
  }
}

unsigned short Being::canSee(Being* target) {
  // Check for immortal invisibility...
  if (target->isAvatar()) {
    if (((Avatar*)target)->adminFlags().test(ADMIN_INCOGNITO) && target->level() > level() && ((Avatar*)target)->isConnected()) {
      return Being::SEE_NOTHING;
    }
  }
  return Being::SEE_NAME;
}

std::string Being::seeName(Being* target, bool capitalize) {
  if (canSee(target) == Being::SEE_NAME) {
    return target->identifiers().shortname();
  } else {
    return capitalize ? "Someone" : "someone";
  }
}

const char* Being::seeReflexivePronoun(Being* target, bool capitalize) {
  switch (canSee(target)) {
    case Being::SEE_NAME:
      switch (target->gender().number()) {
        case MALE:    return capitalize ? "Himself" : "himself";
        case FEMALE:  return capitalize ? "Herself" : "herself";
        default:      return capitalize ? "Itself"  : "itself";
      }
    default:
      return capitalize ? "Themself" : "themself";
  }
}

const char* Being::seeItemPronoun(Being* target, bool capitalize) {
  switch (canSee(target)) {
    case Being::SEE_NAME:
      switch (target->gender().number()) {
        case MALE:    return capitalize ? "Him" : "him";
        case FEMALE:  return capitalize ? "Her" : "her";
        default:      return capitalize ? "It"  : "it";
      }
    default:
      return capitalize ? "Someone" : "someone";
  }
}

const char* Being::seePosessivePronoun(Being* target, bool capitalize) {
  switch (canSee(target)) {
    case Being::SEE_NAME:
      switch (target->gender().number()) {
        case MALE:    return capitalize ? "His" : "his";
        case FEMALE:  return capitalize ? "Her" : "her";
        default:      return capitalize ? "Its" : "its";
      }
    default:
      return capitalize ? "Their" : "their";
  }
}

unsigned short Being::canSee(Item* target) {
  return Being::SEE_NAME;
}

std::string Being::seeName(Item* target, bool capitalize) {
  if (canSee(target) == Being::SEE_NAME) {
    return target->identifiers().shortname();
  } else {
    return capitalize ? "Something" : "something";
  }
}

bool Being::canAlter(Being* target) {
  return target->level() < level() - ALTERABILITY_LEVEL_DIFFERENCE;
}

bool Being::canMove(const unsigned short& direction, std::string& message) {
  Exit* exit = NULL;

  if ((exit = room()->exit(direction)) == NULL) {
    message.append("You cannot go that way.");
    return false;
  }

  if (exit->flags().test(EXIT_CLOSED)) {
    message.append("That door is closed.");
    return false;
  }

  return true;
}

bool Being::move(const unsigned short& direction) {
  // For standard movement...
  Exit* exit = room()->exit(direction);
  Room* from = room();
  Room* to = exit->targetRoom();
  CmdLook look;
  std::vector<std::string> look_args(1);
  // For group movement...
  Being* member = NULL;
  std::string message;

  if (!deplete_stamina(1)) return false;

  // Make the switch...
  from->remove(this);
  to->add(this);
  room(to);

  // Send some output...
  send("You leave %s.\n", Exit::name(direction));
  switch (direction) {
    case NORTH:
    case EAST:
    case SOUTH:
    case WEST:
      from->send_cond("$a leaves to the $e.", this, exit);
      to->send_cond("$a has arrived from the $n.", this, exit);
      break;
    case UP:
      from->send_cond("$a leaves up.", this, exit);
      to->send_cond("$a has arrived from below.", this, exit);
      break;
    case DOWN:
      from->send_cond("$a leaves down.", this, exit);
      to->send_cond("$a has arrived from above.", this, exit);
      break;
    default:
      break;
  }
  look.execute(this, look_args);

  // If this is the leader of a non-empty Group, invoke movement in the other
  // members as well.
  if (group()->leader() == this && group()->members().size() > 1) {
    for (std::set<Being*>::iterator it = group()->members().begin(); it != group()->members().end(); ++it) {
      member = *it;
      // Don't follow yourself.
      if (member == this) continue;
      // Only those with you can follow you.
      if (member->room() != from) continue;
      // Can we go that way?
      if (member->canMove(direction, message)) {
        member->move(direction);
      } else {
        send("%s could not follow you here.\n", member->identifiers().shortname().c_str());
        member->send("You cannot follow %s %s.\n", identifiers().shortname().c_str(), Exit::name(direction));
        member->send(message);
      }
    }
  }

  return true;
}

// Precondition: the Being has learned the Ability.
unsigned Being::mastery(Ability* ability) {
  std::map<Ability*,unsigned>::const_iterator iter = abilityMastery().find(ability);
  return iter->second;
}

void Being::learn(Ability* ability, unsigned mastery) {
  learned().insert(ability);
  abilityMastery()[ability] = mastery;
  return;
}

void Being::improve(Ability* ability, bool success) {
  if (success) {
    send("You improve upon your '{m%s{x' ability!\n", ability->name().c_str());
  } else {
    send("You learn from your mistakes and your '{m%s{x' improves!\n", ability->name().c_str());
  }
  abilityMastery()[ability] += 1;
  awardExperience(Stats::cone_randomization(level()));
  return;
}

std::set<Ability*> Being::available_abilities(void) const {
  std::set<Ability*> available;
  for (std::set<Ability*>::const_iterator iter = klass()->abilities().abilities().begin(); iter != klass()->abilities().abilities().end(); ++iter) {
    if (can_learn(*iter)) available.insert(*iter);
  }
  return available;
}

bool Being::can_learn(Ability* ability) const {
  // Can't learn something twice.
  if (learned().contains(ability)) return false;
  // You need to be a high enough level.
  if (level() < ability->level()) return false;
  // Are the prerequisites met?
  for (std::set<Ability*>::const_iterator iter = ability->dependencies().begin(); iter != ability->dependencies().end(); ++iter) {
    if (!learned().contains(*iter)) return false;
  }
  // Do you have enough training points?
  if (trains() < ability->trains()) return false;
  // Everything checks out.
  return true;
}

Ability* Being::find_spell(std::string name) const {
  AbilityMap::const_iterator map_iter;
  std::set<Ability*>::const_iterator set_iter;

  // Look for an exact match.
  map_iter = learned().abilitiesByName().find(name);
  if (map_iter != learned().abilitiesByName().end()) return map_iter->second;

  // Look for substring (prefix) matches.
  for (set_iter = learned().abilities().begin(); set_iter != learned().abilities().end(); ++set_iter) {
    if (Regex::strPrefix(name, (*set_iter)->name())) return *set_iter;
  }

  // Nothing.
  return NULL;
}

bool Being::invokeIfLearned(std::string skill_name, Being* target, Item* item) {
  Ability* skill = nullptr;
  if ((skill = learned().find_skill(skill_name))) {
    return skill->invoke(this, target, item);
  }
  return false;
}

bool Being::deplete_mana(unsigned mana_, bool message) {
  if (!check_mana(mana_, message)) return false;
  mana(mana() - mana_);
  return true;
}

bool Being::check_mana(unsigned mana_, bool message) {
  if (mana() < mana_) {
    if (message) send("You don't have enough mana for that.\n");
    return false;
  }
  return true;
}

bool Being::exhausted(void) const {
  return stamina() < 1;
}

bool Being::deplete_stamina(unsigned stamina_, bool message) {
  if (exhausted()) {
    if (message) send("You are exhausted.\n");
    return false;
  }
  if (!check_stamina(stamina_, message)) return false;
  stamina(stamina() - stamina_);
  return true;
}

bool Being::check_stamina(unsigned stamina_, bool message) {
  if (stamina() < stamina_) {
    if (message) send("You don't have the stamina for that.\n");
    return false;
  }
  return true;
}

void Being::masterAllTheThings(void) {
  int loop_guard = 0;
  std::set<Ability*> abilities;
  // Master all learned abilities
  for (auto iter : learned().abilities()) {
    if (mastery(iter) == 100) continue;
    learn(iter, 100);
    send("You have mastered '{m%s{x.\n", iter->name().c_str());
  }
  do {
    if ((abilities = available_abilities()).empty()) break;
    for (auto iter : abilities) {
      learn(iter, 100);
      send("You have learned '{m%s{x.\n", iter->name().c_str());
    }
    if (++loop_guard > 10) {
      ERROR(this, "loop guard hit")
      break;
    }
  } while (!abilities.empty());
  return;
}
