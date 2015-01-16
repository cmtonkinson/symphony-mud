/*
 * $Id: object.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "compoundTable.h"
#include "creature.h"
#include "mysql.h"
#include "object.h"
#include "object-armor.h"
#include "object-clothing.h"
#include "object-container.h"
#include "object-food.h"
#include "object-furniture.h"
#include "object-jewelry.h"
#include "object-key.h"
#include "object-trash.h"
#include "object-weapon.h"
#include "socket.h"

Object::Object(void) {
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

Object::Object(const Object& ref):
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

Object::Object(ROW row) {
  extra(NULL);
  std::vector<std::string> foo = Regex::explode("~", row["composition"], true);
  ID(row["objectID"]);
  type(static_cast<Type>((unsigned)row["type"]), row);
  vnum(row["vnum"]);
  flags().value(row["flags"]);
  for (std::vector<std::string>::iterator it = foo.begin(); it != foo.end(); ++it) {
    composition().insert(CompoundTable::Instance().find(*it));
  }
  level(row["level"]);
  value(row["value"]);
  wearable(static_cast<Wearable>((unsigned)row["wearable"]));
  unserializeModifiers(row["modifiers"]);
  identifiers().unserializeKeywords(row["keywords"]);
  identifiers().shortname(row["shortname"]);
  identifiers().longname(row["longname"]);
  identifiers().description(row["description"]);
  return;
}

Object::~Object(void) {
  for (std::list<Modifier*>::const_iterator it = modifiers().begin(); it != modifiers().end(); ++it) {
    delete *it;
  }
  deleteExtra();
  return;
}

void Object::deleteExtra(void) {
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

/* Chaning Object::_type is a non-trivial operation, since Object::_extra
 * holds a pointer to an Obj____ object, and that pointer is interpreted
 * based on the value of Object::_type.
 */
void Object::type(const Type& type) {
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
    default:                                            break;
  }
  return;
}

void Object::type(const Type& type, ROW row) {
  if (type == _type) {
    return;
  }
  deleteExtra();
  _type = type;
  switch (this->type()) {
    case Type_Armor:      extra(new ObjArmor(row));     break;
    case Type_Clothing:   extra(new ObjClothing(row));  break;
    case Type_Container:  extra(new ObjContainer(row)); break;
    case Type_Food:       extra(new ObjFood(row));      break;
    case Type_Furniture:  extra(new ObjFurniture(row)); break;
    case Type_Jewelry:    extra(new ObjJewelry(row));   break;
    case Type_Key:        extra(new ObjKey(row));       break;
    case Type_Trash:      extra(new ObjTrash(row));     break;
    case Type_Weapon:     extra(new ObjWeapon(row));    break;
    default:                                                break;
  }
  return;
}

void Object::type(const Type& type, void* extra_ptr) {
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
    default:                                                                        break;
  }
  return;
}

const char* Object::typeToString(void) const {
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

void Object::stringToType(const std::string& src) {
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

const char* Object::wearableToString(void) const {
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

void Object::stringToWearable(const std::string& src) {
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

std::string Object::serializeModifiers(void) const {
  std::vector<std::string> foo;
  char buf[128];
  for (std::list<Modifier*>::const_iterator it = modifiers().begin(); it != modifiers().end(); ++it) {
    sprintf(buf, "%s:%d", Creature::attributeToString((*it)->attribute()), (*it)->magnitude());
    foo.push_back(buf);
  }
  return Regex::implode("~", foo);
}

void Object::unserializeModifiers(const std::string& ser) {
  std::vector<std::string> foo = Regex::explode("~", ser);
  std::vector<std::string> bar;
  unsigned short attr = 0;
  int mag = 0;
  for (std::vector<std::string>::iterator it = foo.begin(); it != foo.end(); ++it) {
    bar = Regex::explode(":", *it);
    attr = Creature::stringToAttribute(bar[0]);
    sscanf(bar[1].c_str(), "%d", &mag);
    modifiers().push_back(new Modifier(attr, mag));
  }
  return;
}

std::string Object::implodeComposition(std::string glue) const {
  std::string dest;
  for (std::set<Compound*>::const_iterator it = composition().begin(); it != composition().end(); ++it) {
    dest.append((*it)->identifiers().shortname()).append(glue);
  }
  if (dest.size() > glue.size()) {
    dest.resize(dest.size() - glue.size());
  }
  return dest;
}

std::string Object::decorativeShortname(void) const {
  return listDecorativeFlags().append(identifiers().shortname());
}

std::string Object::decorativeLongname(void) const {
  return listDecorativeFlags().append(identifiers().longname());
}

std::string Object::listDecorativeFlags(void) const {
  std::string output;
  if (glowing()) {
    output.append("{x({cglowing{x) ");
  }
  if (humming()) {
    output.append("{x({yhumming{x) ");
  }
  return output;
}

std::string Object::printStatus(void) const {
  std::string output;
  char buffer[MAX_BUFFER];

  output.append("  --== {Ybasic object data{x ==--\n");
  // Basic object information...
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
    flags().list(FTObject::Instance()).c_str(),
    implodeComposition().c_str(),
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
      sprintf(buffer, "{x%s: {%c%+d{x\n", Creature::attributeToString((*it)->attribute()), (*it)->magnitude() > 0 ? 'G' : 'R', (*it)->magnitude());
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
        sprintf(buffer, "Contains {y%u{x items.\n", container()->inventory().objectList().size());
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

void Object::insert(Mysql* db, const unsigned long& areaID) {
  char query[MAX_BUFFER];
  sprintf(query, "INSERT IGNORE INTO objects (areaID, vnum) VALUES (%lu, %lu);", areaID, vnum());
  db->insert(query);
  ID(db->getInsertID());
  return;
}

void Object::update(Mysql* db) const {
  char query[MAX_BUFFER];
  sprintf(query,
    "UPDATE `objects` SET           \
      `type` = %u,                  \
      `flags` = %lu,                \
      `composition` = '%s',         \
      `level` = %u,                 \
      `value` = %u,                 \
      `wearable` = %u,              \
      `modifiers` = '%s',           \
      `keywords` = '%s',            \
      `shortname` = '%s',           \
      `longname` = '%s',            \
      `description` = '%s',         \
      `furniture_capacity` = %u,    \
      `furniture_lay_on` = %u,      \
      `furniture_sit_at` = %u,      \
      `furniture_sit_on` = %u,      \
      `furniture_stand_on` = %u,    \
      `weapon_type` = %u,           \
      `weapon_verb` = %u,           \
      `weapon_damage_number` = %u,  \
      `weapon_damage_faces` = %u    \
     WHERE `objectID` = %lu         \
     LIMIT 1;",
    type(),
    flags().value(),
    Mysql::addslashes(implodeComposition("~")).c_str(),
    level(),
    value(),
    wearable(),
    Mysql::addslashes(serializeModifiers()).c_str(),
    Mysql::addslashes(identifiers().serializeKeywords()).c_str(),
    Mysql::addslashes(identifiers().shortname()).c_str(),
    Mysql::addslashes(identifiers().longname()).c_str(),
    Mysql::addslashes(identifiers().description()).c_str(),
    isFurniture() ? furniture()->capacity() : 0,
    isFurniture() ? furniture()->layOn() : 0,
    isFurniture() ? furniture()->sitAt() : 0,
    isFurniture() ? furniture()->sitOn() : 0,
    isFurniture() ? furniture()->standOn() : 0,
    isWeapon() ? weapon()->type().number() : 0,
    isWeapon() ? weapon()->verb().number() : 0,
    isWeapon() ? weapon()->damage().number() : 0,
    isWeapon() ? weapon()->damage().faces() : 0,
    ID()
 );
  db->update(query);
  return;
}

void Object::destroy(Mysql* db) const {
  char query[MAX_BUFFER];
  sprintf(query, "DELETE FROM objects WHERE objectID = %lu LIMIT 1;", ID());
  db->remove(query);
  return;
}

void Object::saveInstance(Mysql* db, const std::string& placement, const unsigned long& id, const unsigned& location, const unsigned & order, char* hash) const {
  char this_hash[MAX_BUFFER];
  unsigned internal_order = 0;
  char query[MAX_BUFFER];
  if (hash) {
    sprintf(this_hash, "%s-%s%lu%u%u", hash, placement.c_str(), id, location, order);
  } else {
    sprintf(this_hash, "none-%s%lu%u%u", placement.c_str(), id, location, order);
  }
  sprintf(query,
    "INSERT INTO `object_instances` (\
        `hash`,                 \
        `in`,                   \
        `placement`,            \
        `id`,                   \
        `location`,             \
        `order`,                \
        `objectID`,             \
        `vnum`,                 \
        `type`,                 \
        `flags`,                \
        `composition`,          \
        `level`,                \
        `value`,                \
        `wearable`,             \
        `modifiers`,            \
        `keywords`,             \
        `shortname`,            \
        `longname`,             \
        `description`,          \
        `furniture_capacity`,   \
        `furniture_lay_on`,     \
        `furniture_sit_at`,     \
        `furniture_sit_on`,     \
        `furniture_stand_on`,   \
        `weapon_type`,          \
        `weapon_verb`,          \
        `weapon_damage_number`, \
        `weapon_damage_faces`   \
     ) VALUES (               \
        '%s',                   \
        '%s',                   \
        '%s',                   \
        %lu,                    \
        %u,                     \
        %u,                     \
        %lu,                    \
        %lu,                    \
        %u,                     \
        %lu,                    \
        '%s',                   \
        %u,                     \
        %u,                     \
        %u,                     \
        '%s',                   \
        '%s',                   \
        '%s',                   \
        '%s',                   \
        '%s',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u',                   \
        '%u'                    \
     );",
    Mysql::addslashes(this_hash).c_str(),
    Mysql::addslashes(hash?hash:"none").c_str(),
    Mysql::addslashes(placement).c_str(),
    id,
    location,
    order,
    ID(),
    vnum(),
    type(),
    flags().value(),
    Mysql::addslashes(implodeComposition("~")).c_str(),
    level(),
    value(),
    wearable(),
    Mysql::addslashes(serializeModifiers()).c_str(),
    Mysql::addslashes(identifiers().serializeKeywords()).c_str(),
    Mysql::addslashes(identifiers().shortname()).c_str(),
    Mysql::addslashes(identifiers().longname()).c_str(),
    Mysql::addslashes(identifiers().description()).c_str(),
    isFurniture() ? furniture()->capacity() : 0,
    isFurniture() ? furniture()->layOn() : 0,
    isFurniture() ? furniture()->sitAt() : 0,
    isFurniture() ? furniture()->sitOn() : 0,
    isFurniture() ? furniture()->standOn() : 0,
    isWeapon() ? weapon()->type().number() : 0,
    isWeapon() ? weapon()->verb().number() : 0,
    isWeapon() ? weapon()->damage().number() : 0,
    isWeapon() ? weapon()->damage().faces() : 0
 );
  db->insert(query);
  if (isContainer()) {
    for (std::list<Object*>::const_iterator it = container()->inventory().objectList().begin(); it != container()->inventory().objectList().end(); ++it) {
      (*it)->saveInstance(db, "CONTAINER", id, 0, internal_order++, this_hash);
    }
  }
  return;
}
