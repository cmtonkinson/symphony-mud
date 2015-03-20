
#include "ability.h"
#include "avatar.h"
#include "being.h"
#include "commandTable-default.h"
#include "display.h"
#include "exit.h"
#include "group.h"
#include "identifiers.h"
#include "io-handler.h"
#include "item-types.h"
#include "job.h"
#include "npc.h"
#include "room.h"
#include "stats.h"
#include "world.h"
#include "zone.h"

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
  // Stats
  maxStrength(20);
  strength(15);
  maxDexterity(20);
  dexterity(15);
  maxConstitution(20);
  constitution(15);
  maxIntelligence(20);
  intelligence(15);
  maxFocus(20);
  focus(15);
  maxCreativity(20);
  creativity(15);
  maxCharisma(20);
  charisma(15);
  maxLuck(20);
  luck(15);
  // Armor
  armor(100);
  bash(0);
  slash(0);
  pierce(0);
  exotic(0);
  // Misc
  trains(BASE_TRAINS);
  gold(0);
  silver(0);
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
  maxStrength(ref.maxStrength());
  strength(ref.strength());
  maxDexterity(ref.maxDexterity());
  dexterity(ref.dexterity());
  maxConstitution(ref.maxConstitution());
  constitution(ref.constitution());
  maxIntelligence(ref.maxIntelligence());
  intelligence(ref.intelligence());
  maxFocus(ref.maxFocus());
  focus(ref.focus());
  maxCreativity(ref.maxCreativity());
  creativity(ref.creativity());
  maxCharisma(ref.maxCharisma());
  charisma(ref.charisma());
  maxLuck(ref.maxLuck());
  luck(ref.luck());
  armor(ref.armor());
  bash(ref.bash());
  slash(ref.slash());
  pierce(ref.pierce());
  exotic(ref.exotic());
  trains(ref.trains());
  gold(ref.gold());
  silver(ref.silver());
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
  unsigned short Str = 0;
  unsigned short Dex = 0;
  unsigned short Con = 0;
  unsigned short Int = 0;
  unsigned short Foc = 0;
  unsigned short Cre = 0;
  unsigned short Cha = 0;
  unsigned short Luc = 0;
  switch (race().number()) {
    case ELF:
      Dex += 2;
      Int += 1;
      Cha += 2;
      Con -= 1;
      break;
    case DWARF:
      Str += 2;
      Con += 2;
      Luc += 3;
      Dex -= 1;
      Cha -= 1;
      break;
    case HUMAN:
      break;
  }
  switch (pClass().number()) {
    case CLERIC:
      Foc += 2;
      Cre -= 1;
      Str -= 1;
      break;
    case MAGE:
      Int += 2;
      Foc += 2;
      Cre += 2;
      Str -= 2;
      Con -= 2;
      break;
    case ROGUE:
      Dex += 2;
      Int += 1;
      Cre += 1;
      Con -= 1;
      Cha -= 2;
      break;
    case WARRIOR:
      Str += 2;
      Dex += 1;
      Con += 2;
      Int -= 2;
      Foc -= 2;
      break;
  }
  strength(strength() + Str);
  maxStrength(maxStrength() + Str);
  dexterity(dexterity() + Dex);
  maxDexterity(maxDexterity() + Dex);
  constitution(constitution() + Con);
  maxConstitution(maxConstitution() + Con);
  intelligence(intelligence() + Int);
  maxIntelligence(maxIntelligence() + Int);
  focus(focus() + Foc);
  maxFocus(maxFocus() + Foc);
  creativity(creativity() + Cre);
  maxCreativity(maxCreativity() + Cre);
  charisma(charisma() + Cha);
  maxCharisma(maxCharisma() + Cha);
  luck(luck() + Luc);
  maxLuck(maxLuck() + Luc);
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
  if (name == "self") {
    return this;
  }
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
    return ATTR_MAX_HEALTH;
  } else if (Regex::strPrefix(name, "mana")) {
    return ATTR_MAX_MANA;
  } else if (Regex::strPrefix(name, "move")) {
    return ATTR_MAX_MOVE;
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
  } else {
    return ATTR_END;
  }
}

const char* Being::attributeToString(const unsigned short& index) {
  switch (index) {
    case ATTR_MAX_HEALTH: return "health";
    case ATTR_MAX_MANA:   return "mana";
    case ATTR_MAX_MOVE:   return "move";
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

std::string Being::ident(void) {
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
    case ATTR_MAX_HEALTH: _maxHealth    += magnitude; break;
    case ATTR_MAX_MANA:   _maxMana      += magnitude; break;
    case ATTR_STR:        _strength     += magnitude; break;
    case ATTR_DEX:        _dexterity    += magnitude; break;
    case ATTR_CON:        _constitution += magnitude; break;
    case ATTR_INT:        _intelligence += magnitude; break;
    case ATTR_FOC:        _focus        += magnitude; break;
    case ATTR_CRE:        _creativity   += magnitude; break;
    case ATTR_CHA:        _charisma     += magnitude; break;
    case ATTR_LUC:        _luck         += magnitude; break;
    case ATTR_ARMOR:      _armor        += magnitude; break;
    case ATTR_BASH:       _bash         += magnitude; break;
    case ATTR_SLASH:      _slash        += magnitude; break;
    case ATTR_PIERCE:     _pierce       += magnitude; break;
    case ATTR_EXOTIC:     _exotic       += magnitude; break;
    default: break;
  }
  return;
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
      from->send_cond("$p leaves to the $e.", this, exit);
      to->send_cond("$p has arrived from the $n.", this, exit);
      break;
    case UP:
      from->send_cond("$p leaves up.", this, exit);
      to->send_cond("$p has arrived from below.", this, exit);
      break;
    case DOWN:
      from->send_cond("$p leaves down.", this, exit);
      to->send_cond("$p has arrived from above.", this, exit);
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
