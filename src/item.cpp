
#include "being.hpp"
#include "compound-table.hpp"
#include "item-types.hpp"
#include "item.hpp"
#include "socket.hpp"

Item::Item(void) {
  extra(NULL);
  ID(0);
  type(Type_Undefined);
  vnum(0);
  level(1);
  value(1);
  compound(nullptr);
  wearable(Wearable_Undefined);
  identifiers().shortname("undefined");
  identifiers().longname("undefined");
  return;
}

Item::Item(const Item& ref):
    _flags(ref.flags()),
    _identifiers(ref.identifiers()),
    _primary_compound(ref.compound()),
    _other_compounds(ref.composition()) {
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
 * holds a pointer to an Item____ item, and that pointer is interpreted
 * based on the value of Item::_type.
 */
void Item::type(Type type) {
  if (type == _type) {
    return;
  }
  deleteExtra();
  _type = type;
  switch (this->type()) {
    case Type_Armor:      extra(new ItemArmor());      break;
    case Type_Clothing:   extra(new ItemClothing());   break;
    case Type_Container:  extra(new ItemContainer());  break;
    case Type_Food:       extra(new ItemFood());       break;
    case Type_Furniture:  extra(new ItemFurniture());  break;
    case Type_Jewelry:    extra(new ItemJewelry());    break;
    case Type_Key:        extra(new ItemKey());        break;
    case Type_Trash:      extra(new ItemTrash());      break;
    case Type_Weapon:     extra(new ItemWeapon(this)); break;
    default:                                           break;
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
    case Type_Armor:      extra(new ItemArmor(*((ItemArmor*)extra_ptr)));         break;
    case Type_Clothing:   extra(new ItemClothing(*((ItemClothing*)extra_ptr)));   break;
    case Type_Container:  extra(new ItemContainer(*((ItemContainer*)extra_ptr))); break;
    case Type_Food:       extra(new ItemFood(*((ItemFood*)extra_ptr)));           break;
    case Type_Furniture:  extra(new ItemFurniture(*((ItemFurniture*)extra_ptr))); break;
    case Type_Jewelry:    extra(new ItemJewelry(*((ItemJewelry*)extra_ptr)));     break;
    case Type_Key:        extra(new ItemKey(*((ItemKey*)extra_ptr)));             break;
    case Type_Trash:      extra(new ItemTrash(*((ItemTrash*)extra_ptr)));         break;
    case Type_Weapon:     extra(new ItemWeapon(this, *((ItemWeapon*)extra_ptr))); break;
    default:                                                                      break;
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

std::string Item::serializeCompound(void) const {
  return compound() == nullptr ? "" : compound()->identifiers().shortname();
}

void Item::unserializeCompound(std::string ser) {
  compound(CompoundTable::Instance().find(ser));
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

std::string Item::printInformation(void) const {
  std::string dest;
  char buffer[Socket::MAX_BUFFER];

  sprintf(buffer, "\
+----------------------+----------------------------------------------------+\n\
| vnum      {y%-10ld{x | keywords     {y%-35s{x   |\n\
| type      {y%-10s{x | shortname    {y%-35s{x   |\n\
| level     {y%-10d{x | longname     {y%-35s{x   |\n\
| flags     {y%-10s{x | primary comp {y%-35s{x   |\n\
| value     {y%-10d{x | other comp   {y%-35s{x   |\n\
| wearable  {y%-10s{x |                                                    |\n\
+----------------------+----------------------------------------------------+\n\
| description {y%s{x\n\
+---------------------------------------------------------------------------+\n",
    vnum(), identifiers().getKeywordList().c_str(),
    typeToString(), identifiers().shortname().c_str(),
    level(), identifiers().longname().c_str(),
    flags().list(FTItem::Instance()).c_str(), (compound() == nullptr ? "" : compound()->identifiers().shortname().c_str()),
    value(), serializeComposition().c_str(),
    wearableToString(),
    identifiers().description().c_str()
  );
  dest.append(buffer);

  // Attribute modifiers...
  if (!modifiers().empty()) {
    for (std::list<Modifier*>::const_iterator it = modifiers().begin(); it != modifiers().end(); ++it) {
      sprintf(buffer, "| {x%s {%c%+d{x\n", Being::attributeToString((*it)->attribute()), (*it)->magnitude() > 0 ? 'G' : 'R', (*it)->magnitude());
      dest.append(buffer);
    }
    dest.append("+---------------------------------------------------------------------------+\n");
  }

  if (extra()) {
    // Type-specific data...
    sprintf(buffer, "| {Y%s information{x\n", typeToString());
    dest.append(buffer);
    switch (type()) {
      case Type_Armor:
        break;
      case Type_Clothing:
        break;
      case Type_Container:
        sprintf(buffer, "| Contains {y%zd{x items.\n", container()->inventory().itemList().size());
        dest.append(buffer);
        break;
      case Type_Food:
        break;
      case Type_Furniture:
        sprintf(buffer, "| Overall capacity: {y%u{x\n", furniture()->capacity());
        dest.append(buffer);
        sprintf(buffer, "| Laying capacity: {y%u{x\n", furniture()->layOn());
        dest.append(buffer);
        sprintf(buffer, "| Sitting (at) capacity: {y%u{x\n", furniture()->sitAt());
        dest.append(buffer);
        sprintf(buffer, "| Sitting (on) capacity: {y%u{x\n", furniture()->sitOn());
        dest.append(buffer);
        sprintf(buffer, "| Standing capacity: {y%u{x\n", furniture()->standOn());
        dest.append(buffer);
        break;
      case Type_Jewelry:
        break;
      case Type_Key:
        break;
      case Type_Trash:
        break;
      case Type_Weapon:
        sprintf(buffer, "| type: {y%s{x\n", weapon()->type().string().c_str());
        dest.append(buffer);
        sprintf(buffer, "| verb: {y%s{x\n", weapon()->verb().string().c_str());
        dest.append(buffer);
        sprintf(buffer, "| damage: {y%s{x (average {y%u{x)\n", weapon()->damage().toString(), weapon()->damage().average());
        dest.append(buffer);
        sprintf(buffer, "| key stat: {y%s{x\n", (weapon()->keyStat() == Being::ATTR_UNDEFINED ? "N/A" : Being::attributeToString(weapon()->keyStat())));
        dest.append(buffer);
        break;
      default:
        break;
    }
    dest.append("+---------------------------------------------------------------------------+\n");
  }

  return dest;
}
