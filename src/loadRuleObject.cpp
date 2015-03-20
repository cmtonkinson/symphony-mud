
#include "commandTable-default.h"
#include "loadRuleObject.h"
#include "room.h"
#include "world.h"

LoadRuleObject::LoadRuleObject(void) {
  type(OBJECT);
  preposition(ERROR);
  indirectObject(ERROR);
  indirectObjectIndex(ERROR);
  return;
}

LoadRuleObject::~LoadRuleObject(void) {
  return;
}

std::string LoadRuleObject::notes(void) const {
  std::string foo;

  switch (preposition()) {
    case ON:
      foo.assign("on object ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case IN:
      foo.assign("in object ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case CARRY:
      foo.assign("carried by mob ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    case WEAR:
      foo.assign("worn by mob ").append(estring(indirectObject())).append("#").append(estring(indirectObjectIndex()));
      break;
    default:
      foo.assign("N/A");
      break;
  }

  return foo;
}

void LoadRuleObject::destroy(void) {
  delete this;
  return;
}

bool LoadRuleObject::execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs) {
  std::map<unsigned long,Object*>::iterator it;
  Area* area = room()->area();
  Object* object = NULL;
  Being* being = NULL;
  unsigned already_there = room()->inventory().howManyObjects(target());
  unsigned objects_added = 0;
  CmdGet get;
  CmdWear wear;
  std::vector<std::string> args(1);

  if ((it = area->objects().find(target())) != area->objects().end()) {
    object = it->second;
  } else {
    World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset an object. Object %lu in area %lu doesn't exist.", target(), area->ID());
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
      if (preposition() == CARRY || preposition() == WEAR) {
        being = room()->being_by_vnum(indirectObject(), indirectObjectIndex());
        if (being == NULL) {
          World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset an object. Object %lu in area %lu loaded, but can't find mob %lu#%hu ", target(), area->ID(), indirectObject(), indirectObjectIndex());
          return false;
        }
        args[0] = object->identifiers().longestKeyword();
        // Pick up the item.
        get.execute(being, args);
        // Wear it, if applicable.
        if (preposition() == WEAR) wear.execute(being, args);
      }
    }
  }

  return true;
}
