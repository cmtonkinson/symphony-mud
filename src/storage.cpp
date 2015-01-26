
#include <cstring>
#include "area.h"
#include "enumTable.h"
#include "exit.h"
#include "loadRule.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "object-furniture.h"
#include "object-weapon.h"
#include "object.h"
#include "room.h"
#include "storage.h"

const unsigned Storage::LOAD_DONE;
const unsigned Storage::LOAD_NULL;
const unsigned Storage::LOAD_NEW;

/***************************************************************************************************
 * AREA
 **************************************************************************************************/
void Storage::dump(FILE* fp, Area* area) {
  BEGIN("AREA")
  out(fp, "low",          area->low());
  out(fp, "high",         area->high());
  out(fp, "name",         area->name());
  out(fp, "terrain",      area->terrain()->name());
  END("AREA")
  return;
}

bool Storage::load(FILE* fp, Area* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "AREA", [&fp, &loading, &input]() {
    STORE_CASE("low",       &Area::low)
    STORE_CASE("high",      &Area::high)
    STORE_CASE("name",      &Area::name)
    STORE_CASE("terrain",   &Area::setTerrain)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * ROOM
 **************************************************************************************************/
void Storage::dump(FILE* fp, Room* room) {
  BEGIN("ROOM")
  out(fp, "vnum",         room->vnum());
  out(fp, "name",         room->name());
  out(fp, "description",  room->description());
  out(fp, "smell",        room->smell());
  out(fp, "sound",        room->sound());
  out(fp, "terrain",      room->terrain()->name());
  for (unsigned u = 0; u < 6; ++u) {
    if (room->exit(u)) dump(fp, room->exit(u));
  }
  for (auto iter : room->loadRules()) {
    dump(fp, iter);
  }
  END("ROOM\n")
  return;
}

bool Storage::load(FILE* fp, Room* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "ROOM", [&fp, &loading, &input]() {
    STORE_CASE("vnum",        &Room::vnum)
    STORE_CASE("name",        &Room::name)
    STORE_CASE("description", &Room::description)
    STORE_CASE("smell",       &Room::smell)
    STORE_CASE("sound",       &Room::sound)
    STORE_CASE("terrain",     &Room::setTerrain)
    STORE_DESCEND("EXIT", Exit,
      loading->exit(instance->direction().number(), instance);
    )
    STORE_DESCEND("RULE_OBJ", LoadRuleObject,
      loading->loadRules().push_back(instance);
      instance->room(loading);
    )
    STORE_DESCEND("RULE_MOB", LoadRuleMob,
      loading->loadRules().push_back(instance);
      instance->room(loading);
    )
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * EXIT
 **************************************************************************************************/
void Storage::dump(FILE* fp, Exit* exit) {
  BEGIN("EXIT")
  out(fp, "direction",  exit->direction());
  out(fp, "key",        exit->key());
  END("EXIT")
  return;
}

bool Storage::load(FILE* fp, Exit* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "EXIT", [&fp, &loading, &input]() {
    STORE_CASE("direction", &Exit::direction)
    STORE_CASE("key",       &Exit::key)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * LOAD RULES
 **************************************************************************************************/
void Storage::dump(FILE* fp, LoadRule* rule) {
  char buffer[32];
  sprintf(buffer, "RULE_%s", rule->strType());
  BEGIN(buffer)
  out(fp, "target",       rule->target());
  out(fp, "number",       rule->number());
  out(fp, "max",          rule->max());
  out(fp, "probability",  rule->probability());
  switch (rule->type()) {
    case LoadRule::OBJECT:
      out(fp, "preposition",          ((LoadRuleObject*)rule)->preposition());
      out(fp, "indirectObject",       ((LoadRuleObject*)rule)->indirectObject());
      out(fp, "indirectObjectIndex",  ((LoadRuleObject*)rule)->indirectObjectIndex());
      break;
  }
  END(buffer)
  return;
}

bool Storage::load(FILE* fp, LoadRule* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "RULE_", [&fp, &loading, &input]() {
    STORE_CASE("target",              &LoadRule::target)
    STORE_CASE("number",              &LoadRuleObject::number)
    STORE_CASE("max",                 &LoadRuleObject::max)
    STORE_CASE("probability",         &LoadRuleObject::probability)
    switch (loading->type()) {
      case LoadRule::OBJECT:
        STORE_CASE_WITH_TYPE("preposition",         &LoadRuleObject::preposition,         LoadRuleObject)
        STORE_CASE_WITH_TYPE("indirectObject",      &LoadRuleObject::indirectObject,      LoadRuleObject)
        STORE_CASE_WITH_TYPE("indirectObjectIndex", &LoadRuleObject::indirectObjectIndex, LoadRuleObject)
        break;
    }
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * OBJECT
 **************************************************************************************************/
void Storage::dump(FILE* fp, Object* object) {
  BEGIN("OBJECT")
  out(fp, "vnum",         object->vnum());
  out(fp, "level",        object->level());
  out(fp, "value",        object->value());
  out(fp, "type",         object->type());
  out(fp, "wearable",     object->wearable());
  out(fp, "flags",        object->flags().value());
  out(fp, "shortname",    object->identifiers().shortname());
  out(fp, "longname",     object->identifiers().longname());
  out(fp, "description",  object->identifiers().description());
  out(fp, "keywords",     object->identifiers().serializeKeywords());
  out(fp, "modifiers",    object->serializeModifiers());
  out(fp, "composition",  object->serializeComposition());
  switch (object->type()) {
    case Object::Type_Furniture:
      out(fp, "furniture_capacity", object->furniture()->capacity());
      out(fp, "furniture_layOn",    object->furniture()->layOn());
      out(fp, "furniture_sitAt",    object->furniture()->sitAt());
      out(fp, "furniture_sitOn",    object->furniture()->sitOn());
      out(fp, "furniture_standOn",  object->furniture()->standOn());
      break;
    case Object::Type_Weapon:
      out(fp, "weapon_type",     object->weapon()->type().string());
      out(fp, "weapon_verb",     object->weapon()->verb().string());
      out(fp, "weapon_damage",   object->weapon()->damage().serialize());
      break;
    default:
      break;
  }
  END("OBJECT")
  return;
}

bool Storage::load(FILE* fp, Object* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "OBJECT", [&fp, &loading, &input]() {
    STORE_CASE("vnum",                &Object::vnum)
    STORE_CASE("level",               &Object::level)
    STORE_CASE("value",               &Object::value)
    STORE_CASE_WITH_CODE("type",      unsigned,       "%u",   loading->type(static_cast<Object::Type>(val));)
    STORE_CASE_WITH_CODE("wearable",  unsigned,       "%u",   loading->wearable(static_cast<Object::Wearable>(val));)
    STORE_CASE_WITH_CODE("flags",     unsigned long,  "%lu",  loading->flags().value(val);)
    STORE_CASE_STRING("shortname",    loading->identifiers().shortname(str);)
    STORE_CASE_STRING("longname",     loading->identifiers().longname(str);)
    STORE_CASE_STRING("description",  loading->identifiers().description(str);)
    STORE_CASE_STRING("keywords",     loading->identifiers().unserializeKeywords(str);)
    STORE_CASE_STRING("modifiers",    loading->unserializeModifiers(str);)
    STORE_CASE_STRING("composition",  loading->unserializeComposition(str);)
    switch (loading->type()) {
      case Object::Type_Furniture:
        STORE_CASE_WITH_CODE("furniture_capacity",  unsigned, "%u", loading->furniture()->capacity(val);)
        STORE_CASE_WITH_CODE("furniture_layOn",     unsigned, "%u", loading->furniture()->layOn(val);)
        STORE_CASE_WITH_CODE("furniture_sitAt",     unsigned, "%u", loading->furniture()->sitAt(val);)
        STORE_CASE_WITH_CODE("furniture_sitOn",     unsigned, "%u", loading->furniture()->sitOn(val);)
        STORE_CASE_WITH_CODE("furniture_standOn",   unsigned, "%u", loading->furniture()->standOn(val);)
        break;
      case Object::Type_Weapon:
        STORE_CASE_STRING("weapon_type",    loading->weapon()->type().set(ETWeaponType::Instance().get(str));)
        STORE_CASE_STRING("weapon_verb",    loading->weapon()->verb().set(ETDamageVerb::Instance().get(str));)
        STORE_CASE_STRING("weapon_damage",  loading->weapon()->damage().unserialize(str);)
        break;
      default:
        break;
    }
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * INTERNAL METHODS
 **************************************************************************************************/
unsigned Storage::load_inner(FILE* fp, void* loading, char* input, const char* boundary, voidFunc lambda) {
  int next = 0;
  char end_boundary[32];
  sprintf(end_boundary, "/%s", boundary);

  // The first input should be a prefix match on the boundary for the object.
  fscanf(fp, "%s", input);
  if (feof(fp) || strstr(input, boundary) == NULL) return LOAD_NULL;

  // Iteratively look for new keys.
  while (!feof(fp)) {
    // Get the input.
    fscanf(fp, "%s", input);
    // Stop if this is the end token.
    if (strstr(input, end_boundary) != NULL) break;
    // Look for valid keys.
    lambda();
    // Test the stream (and reset the pointer).
    next = peek(fp)[0];
    // If the next character is capitalized, switch modes.
    if (std::isupper(next)) return LOAD_NEW;
    // The next input looks like a key - keep iterating.
  }
  return LOAD_DONE;
}

std::string Storage::read_string(FILE* fp) {
  std::string output;
  unsigned length = 0;
  char* buffer    = nullptr;
  // Find the length of the string and allocate a buffer.
  fscanf(fp, "%zu:", &length);
  if (length < 1) return ""; // Empty string
  buffer = new char[length + 1];
  // Read the string and terminate with null.
  fgets(buffer, length + 1, fp);
  buffer[length] = 0;
  // Convert the data to a string and clean up.
  output = buffer;
  delete buffer;
  return output;
}

std::string Storage::peek(FILE* fp) {
  char buf[32];
  fgets(buf, 32, fp);
  fseek(fp, -strlen(buf), SEEK_CUR);
  return buf;
}

void Storage::out(FILE* fp, const char* key, const char* value) {
  fprintf(fp, "%s %u:%s\n", key, strlen(value), value);
  return;
}

template <class ObjectType>
void Storage::in(FILE* fp, ObjectType* object, void (ObjectType::*method)(const char*)) {
  std::string buf = read_string(fp);
  (*object.*method)(buf.c_str());
  return;
}

template <class ObjectType>
void Storage::in(FILE* fp, ObjectType* object, void (ObjectType::*method)(std::string)) {
  std::string buf = read_string(fp);
  (*object.*method)(buf);
  return;
}
