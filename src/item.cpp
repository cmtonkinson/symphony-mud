
#include "compoundTable.h"
#include "being.h"
#include "item-armor.h"
#include "item-clothing.h"
#include "item-container.h"
#include "item-food.h"
#include "item-furniture.h"
#include "item-jewelry.h"
#include "item-key.h"
#include "item-trash.h"
#include "item-weapon.h"
#include "item.h"
#include "socket.h"

Item::Item(void) {
  extra(NULL);
  ID(0);
  type(Type_Undefined);
  vnum(0);
  level(1);
  value(1);
  wearable(Wearable_Undefined);
  identifiers().shortname("undefined");
  identifiers().longname("undefined");
  return;
}

Item::Item(const Item& ref):
    _flags(ref.flags()),
    _identifiers(ref.identifiers()),
    _composition(ref.composition()) {
  extra(NULL);
  ID(ref.ID());
  type(ref.type(), ref.extra());
  vnum(ref.vnum());
  level(ref.level());
  value(ref.value());
  wearable(ref.wearable());
  for (std::list<Modifier*>::const_iterator it = ref.modifiers().begin(); it != ref.modifiers().end(); ++it) {
    modifiers().push_back(new Modifier(**it));
  }
  return;
}

Item::~Item(void) {
  for (std::list<Modifier*>::const_iterator it = modifiers().begin(); it != modifiers().end(); ++it) {
    delete *it;
  }
  deleteExtra();
  return;
}

void Item::deleteExtra(void) {
  if (extra()) {
    switch (type()) {
      case Type_Armor:      delete armor();     break;
      case Type_Clothing:   delete clothing();  break;
      case Type_Container:  delete container(); break;
      case Type_Food:       delete food();      break;
      case Type_Furniture:  delete furniture(); break;
      case Type_Jewelry:    delete jewelry();   break;
      case Type_Key:        delete key();       break;
      case Type_Trash:      delete trash();     break;
      case Type_Weapon:     delete weapon();    break;
      default:                                  break;
    }
  }
  _extra = NULL;
  _type = Type_Undefined;
}

/* Changing Item::_type is a non-trivial operation, since Item::_extra
 * holds a pointer to an Obj____ item, and that pointer is interpreted
 * based on the value of Item::_type.
 */
void Item::type(Type type) {
  if (type == _type) {
    return;
  }
  deleteExtra();
  _type = type;
  switch (this->type()) {
    case Type_Armor:      extra(new ObjArmor());      break;
    case Type_Clothing:   extra(new ObjClothing());   break;
    case Type_Container:  extra(new ObjContainer());  break;
    case Type_Food:       extra(new ObjFood());       break;
    case Type_Furniture:  extra(new ObjFurniture());  break;
    case Type_Jewelry:    extra(new ObjJewelry());    break;
    case Type_Key:        extra(new ObjKey());        break;
    case Type_Trash:      extra(new ObjTrash());      break;
    case Type_Weapon:     extra(new ObjWeapon());     break;
    default:                                          break;
  }
  return;
}

void Item::type(Type type, void* extra_ptr) {
  if (type == _type) {
    return;
  }
  deleteExtra();
  _type = type;
  switch (this->type()) {
    case Type_Armor:      extra(new ObjArmor(*((ObjArmor*)extra_ptr)));         break;
    case Type_Clothing:   extra(new ObjClothing(*((ObjClothing*)extra_ptr)));   break;
    case Type_Container:  extra(new ObjContainer(*((ObjContainer*)extra_ptr))); break;
    case Type_Food:       extra(new ObjFood(*((ObjFood*)extra_ptr)));           break;
    case Type_Furniture:  extra(new ObjFurniture(*((ObjFurniture*)extra_ptr))); break;
    case Type_Jewelry:    extra(new ObjJewelry(*((ObjJewelry*)extra_ptr)));     break;
    case Type_Key:        extra(new ObjKey(*((ObjKey*)extra_ptr)));             break;
    case Type_Trash:      extra(new ObjTrash(*((ObjTrash*)extra_ptr)));         break;
    case Type_Weapon:     extra(new ObjWeapon(*((ObjWeapon*)extra_ptr)));       break;
    default:                                                                    break;
  }
  return;
}

const char* Item::typeToString(void) const {
  switch (type()) {
    case Type_Armor:      return "armor";
    case Type_Clothing:   return "clothing";
    case Type_Container:  return "container";
    case Type_Food:       return "food";
    case Type_Furniture:  return "furniture";
    case Type_Jewelry:    return "jewelry";
    case Type_Key:        return "key";
    case Type_Trash:      return "trash";
    case Type_Weapon:     return "weapon";
    default:              return "undefined";
  }
}

void Item::stringToType(const std::string& src) {
  if (Regex::strPrefix(src, "armor")) {
    type(Type_Armor);
  } else if (Regex::strPrefix(src, "clothing")) {
    type(Type_Clothing);
  } else if (Regex::strPrefix(src, "container")) {
    type(Type_Container);
  } else if (Regex::strPrefix(src, "food")) {
    type(Type_Food);
  } else if (Regex::strPrefix(src, "furniture")) {
    type(Type_Furniture);
  } else if (Regex::strPrefix(src, "jewelry")) {
    type(Type_Jewelry);
  } else if (Regex::strPrefix(src, "key")) {
    type(Type_Key);
  } else if (Regex::strPrefix(src, "trash")) {
    type(Type_Trash);
  } else if (Regex::strPrefix(src, "weapon")) {
    type(Type_Weapon);
  } else {
    type(Type_Undefined);
  }
  return;
}

const char* Item::wearableToString(void) const {
  switch (wearable()) {
    case Wearable_Head:       return "head";
    case Wearable_Ear:        return "ear";
    case Wearable_Face:       return "face";
    case Wearable_Neck:       return "neck";
    case Wearable_Shoulders:  return "shoulders";
    case Wearable_Arms:       return "arms";
    case Wearable_Torso:      return "torso";
    case Wearable_Forearm:    return "forearm";
    case Wearable_Wrist:      return "wrist";
    case Wearable_Hands:      return "hands";
    case Wearable_Hold:       return "hold";
    case Wearable_Finger:     return "finger";
    case Wearable_Waist:      return "waist";
    case Wearable_Legs:       return "legs";
    case Wearable_Knee:       return "knee";
    case Wearable_Shin:       return "shin";
    case Wearable_Ankle:      return "ankle";
    case Wearable_Feet:       return "feet";
    default:                  return "undefined";
  }
}

void Item::stringToWearable(const std::string& src) {
  if (Regex::strPrefix(src, "head")) {
    wearable(Wearable_Head);
  } else if (Regex::strPrefix(src, "ear")) {
    wearable(Wearable_Ear);
  } else if (Regex::strPrefix(src, "face")) {
    wearable(Wearable_Face);
  } else if (Regex::strPrefix(src, "neck")) {
    wearable(Wearable_Neck);
  } else if (Regex::strPrefix(src, "shoulders")) {
    wearable(Wearable_Shoulders);
  } else if (Regex::strPrefix(src, "arms")) {
    wearable(Wearable_Arms);
  } else if (Regex::strPrefix(src, "torso")) {
    wearable(Wearable_Torso);
  } else if (Regex::strPrefix(src, "forearm")) {
    wearable(Wearable_Forearm);
  } else if (Regex::strPrefix(src, "wrist")) {
    wearable(Wearable_Wrist);
  } else if (Regex::strPrefix(src, "hands")) {
    wearable(Wearable_Hands);
  } else if (Regex::strPrefix(src, "hold")) {
    wearable(Wearable_Hold);
  } else if (Regex::strPrefix(src, "finger")) {
    wearable(Wearable_Finger);
  } else if (Regex::strPrefix(src, "waist")) {
    wearable(Wearable_Waist);
  } else if (Regex::strPrefix(src, "legs")) {
    wearable(Wearable_Legs);
  } else if (Regex::strPrefix(src, "knee")) {
    wearable(Wearable_Knee);
  } else if (Regex::strPrefix(src, "shin")) {
    wearable(Wearable_Shin);
  } else if (Regex::strPrefix(src, "ankle")) {
    wearable(Wearable_Ankle);
  } else if (Regex::strPrefix(src, "feet")) {
    wearable(Wearable_Feet);
  } else {
    wearable(Wearable_Undefined);
  }
  return;
}

std::string Item::serializeModifiers(void) const {
  std::vector<std::string> foo;
  char buf[128];
  for (auto iter : modifiers()) {
    sprintf(buf, "%s:%d", Being::attributeToString(iter->attribute()), iter->magnitude());
    foo.push_back(buf);
  }
  return Regex::implode("~", foo);
}

void Item::unserializeModifiers(std::string ser) {
  std::vector<std::string> foo = Regex::explode("~", ser);
  std::vector<std::string> bar;
  unsigned short attr = 0;
  int mag = 0;
  for (auto iter : foo) {
    bar = Regex::explode(":", iter);
    attr = Being::stringToAttribute(bar[0]);
    sscanf(bar[1].c_str(), "%d", &mag);
    modifiers().push_back(new Modifier(attr, mag));
  }
  return;
}

std::string Item::serializeComposition(std::string sep) const {
  std::set<std::string> foo;
  for (auto iter : composition()) foo.insert(iter->identifiers().shortname());
  return Regex::implode(sep, foo);
}

void Item::unserializeComposition(std::string ser) {
  std::vector<std::string> foo = Regex::explode("~", ser);
  for (auto iter : foo) composition().insert(CompoundTable::Instance().find(iter));
  return;
}

std::string Item::decorativeShortname(void) const {
  return listDecorativeFlags().append(identifiers().shortname());
}

std::string Item::decorativeLongname(void) const {
  return listDecorativeFlags().append(identifiers().longname());
}

std::string Item::listDecorativeFlags(void) const {
  std::string output;
  if (glowing()) {
    output.append("{x({cglowing{x) ");
  }
  if (humming()) {
    output.append("{x({yhumming{x) ");
  }
  return output;
}

std::string Item::printStatus(void) const {
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  output.append("  --== {Ybasic item data{x ==--\n");
  // Basic item information...
  sprintf(buffer, "vnum......... {y%lu{x\n\
type......... {y%s{x\n\
flags........ {y%s{x\n\
composition.. {y%s{x\n\
level........ {y%u{x\n\
value........ {y%u{x\n\
wearable..... {y%s{x\n\
keywords..... {y%s{x\n\
shortname.... %s\n\
longname..... %s\n\n\
  --== {Ydescription{x ==--\n%s\n\
",  vnum(),
    typeToString(),
    flags().list(FTItem::Instance()).c_str(),
    serializeComposition().c_str(),
    level(),
    value(),
    wearableToString(),
    identifiers().getKeywordList().c_str(),
    identifiers().shortname().c_str(),
    identifiers().longname().c_str(),
    identifiers().description().c_str()
 );
  output.append(buffer);

  // Attribute modifiers...
  if (!modifiers().empty()) {
    output.append("\n  --== {Yattribute modifiers ==--\n");
    for (std::list<Modifier*>::const_iterator it = modifiers().begin(); it != modifiers().end(); ++it) {
      sprintf(buffer, "{x%s: {%c%+d{x\n", Being::attributeToString((*it)->attribute()), (*it)->magnitude() > 0 ? 'G' : 'R', (*it)->magnitude());
      output.append(buffer);
    }
  }

  if (extra()) {
    // Type-specific data...
    sprintf(buffer, "\n  --== {Y%s configuration{x ==--\n", typeToString());
    output.append(buffer);
    switch (type()) {
      case Type_Armor:
        break;
      case Type_Clothing:
        break;
      case Type_Container:
        sprintf(buffer, "Contains {y%zd{x items.\n", container()->inventory().itemList().size());
        output.append(buffer);
        break;
      case Type_Food:
        break;
      case Type_Furniture:
        sprintf(buffer, "Total overall capacity: {y%u{x\n", furniture()->capacity());
        output.append(buffer);
        sprintf(buffer, "Laying capacity: {y%u{x\n", furniture()->layOn());
        output.append(buffer);
        sprintf(buffer, "Sitting (at) capacity: {y%u{x\n", furniture()->sitAt());
        output.append(buffer);
        sprintf(buffer, "Sitting (on) capacity: {y%u{x\n", furniture()->sitOn());
        output.append(buffer);
        sprintf(buffer, "Standing capacity: {y%u{x\n", furniture()->standOn());
        output.append(buffer);
        break;
      case Type_Jewelry:
        break;
      case Type_Key:
        break;
      case Type_Trash:
        break;
      case Type_Weapon:
        sprintf(buffer, "Weapon type: {y%s{x\n", weapon()->type().string().c_str());
        output.append(buffer);
        sprintf(buffer, "Weapon verb: {y%s{x\n", weapon()->verb().string().c_str());
        output.append(buffer);
        sprintf(buffer, "Weapon damage: {y%s{x (average {y%u{x)\n", weapon()->damage().toString(), weapon()->damage().average());
        output.append(buffer);
        break;
      default:
        break;
    }
  }

  return output;
}
