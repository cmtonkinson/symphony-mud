
#include "loadRuleObject.h"
#include "room.h"
#include "world.h"
#include "commandTable-default.h"

LoadRuleObject::LoadRuleObject(void) {
  preposition(LOADRULE_ERROR);
  indirectObject(LOADRULE_ERROR);
  indirectObjectIndex(LOADRULE_ERROR);
  return;
}

LoadRuleObject::LoadRuleObject(ROW row): LoadRule(row) {
  if (row["preposition"] == "ON") {
    preposition(LOADRULE_ON);
  } else if (row["preposition"] == "IN") {
    preposition(LOADRULE_IN);
  } else if (row["preposition"] == "CARRY") {
    preposition(LOADRULE_CARRY);
  } else if (row["preposition"] == "WEAR") {
    preposition(LOADRULE_WEAR);
  } else {
    preposition(LOADRULE_ERROR);
  }
  indirectObject(row["indirect_object"]);
  indirectObjectIndex(row["indirect_object_index"]);
  return;
}

LoadRuleObject::~LoadRuleObject(void) {
  return;
}

std::string LoadRuleObject::notes(void) const {
  std::string foo;

  switch (preposition()) {
    case LOADRULE_ON:
      foo.assign("on object ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case LOADRULE_IN:
      foo.assign("in object ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case LOADRULE_CARRY:
      foo.assign("carried by mob ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case LOADRULE_WEAR:
      foo.assign("worn by mob ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    default:
      foo.assign("N/A");
      break;
  }

  return foo;
}

bool LoadRuleObject::save(void) {
  return false;
}

bool LoadRuleObject::commit(void) {
  std::string prep;

  switch (preposition()) {
    case LOADRULE_IN:    prep = "IN";    break;
    case LOADRULE_ON:    prep = "ON";    break;
    case LOADRULE_CARRY: prep = "CARRY"; break;
    case LOADRULE_WEAR:  prep = "WEAR";  break;
    default:             prep = "NONE";  break;
  }

  try {
    char query[MAX_BUFFER];

    sprintf(
      query,
      "INSERT IGNORE INTO load_rules (vnum, type, target, number, max, probability, preposition, indirect_object, indirect_object_index) VALUES (%lu, '%s', %lu, %u, %u, %u, '%s', %lu, %u);",
      vnum(),
      (type() == LOADRULE_MOB ? "MOB" : "OBJECT"),
      target(),
      number(),
      max(),
      probability(),
      prep.c_str(),
      indirectObject(),
      indirectObjectIndex()
  );
    World::Instance().getMysql()->insert(query);
    ID(World::Instance().getMysql()->getInsertID());

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to commit load_rule vnum %lu: %s\n", vnum(), me.getMessage().c_str());
    return false;
  }

  return true;
}

bool LoadRuleObject::execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs) {
  std::map<unsigned long,Object*>::iterator it;
  Area* area = room()->area();
  Object* object = NULL;
  Creature* creature = NULL;
  unsigned already_there = room()->inventory().howManyObjects(target());
  unsigned objects_added = 0;
  CmdGet get;
  CmdWear wear;
  std::vector<std::string> args(1);

  if ((it = area->objects().find(target())) != area->objects().end()) {
    object = it->second;
  } else {
    World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset an object.  Object %lu in area %lu doesn't exist.", target(), area->ID());
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + objects_added >= max()) {
      break;
    }
    if (World::rand(1, 100) <= probability()) {
      object = new Object(*object);
      World::Instance().insert(object);
      room()->inventory().add(object);
      new_objects.push_back(object);
      objects_added++;
      if (preposition() == LOADRULE_CARRY || preposition() == LOADRULE_WEAR) {
        creature = room()->creature_by_vnum(indirectObject(), indirectObjectIndex());
        if (creature == NULL) {
          World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset an object. Object %lu in area %lu loaded, but can't find mob %lu#%hu ", target(), area->ID(), indirectObject(), indirectObjectIndex());
          return false;
        }
        args[0] = object->identifiers().longestKeyword();
        // Pick up the item.
        get.execute(creature, args);
        // Wear it, if applicable.
        if (preposition() == LOADRULE_WEAR) wear.execute(creature, args);
      }
    }
  }

  return true;
}
