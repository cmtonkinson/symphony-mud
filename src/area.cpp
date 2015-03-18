
#include "area.h"
#include "exit.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "room.h"
#include "storage.h"
#include "terrainTable.h"
#include "world.h"

Area::Area(void) {
  return;
}

Area::Area(unsigned long low, unsigned long high) {
  name("Undefined");
  this->low(low);
  this->high(high);
  terrain(TerrainTable::Instance().find("city"));
}

Area::~Area(void) {
  return;
}

void Area::name(const char* name) {
  ColorString cs(name);
  _name = cs.stripColor();
  return;
}

void Area::initialize(void) {
  World::Instance().schedule()->add(new RecurringJob(this, &Area::reset, 300, 600));
  return;
}

void Area::save(void) {
  std::string filename = "data/areas/" + Regex::slugify(name()) + ".area.txt";
  FILE* fp = 0;

  if ((fp = fopen(filename.c_str(), "w")) != NULL) {
    Storage::dump(fp, this);
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to open %s.\n", filename.c_str());
  }

  return;
}

bool Area::destroy(void) {
  // Destroy rooms.
  while (rooms().size()) {
    if (!rooms().begin()->second->destroy()) {
      World::worldLog(World::LOG_LEVEL_ERROR, World::LOG_TYPE_WORLD, "Room %lu failed to destroy itself with area %lu (%s).", rooms().begin()->second->vnum(), ID(), name().c_str());
      return false;
    }
  }
  // Remove this from the World.
  World::Instance().remove(this);
  // Delete the area file.
  std::string filename = "data/areas/" + Regex::slugify(name()) + ".area.txt";
  remove(filename.c_str());
  // Self-delete.
  delete this;
  // Success!
  return true;
}

void Area::insert(Room* room) {
  room->area(this);
  _rooms.insert(std::make_pair(room->vnum(), room));
  return;
}

void Area::insert(Object* object) {
  _objects.insert(std::make_pair(object->vnum(), object));
  return;
}

void Area::insert(Mob* mob) {
  _mobs.insert(std::make_pair(mob->vnum(), mob));
  return;
}

unsigned long Area::lowestAvailableRoom(void) {
  unsigned long vnum = 0;

  for (unsigned long u = low(); u <= high(); ++u) {
    if (World::Instance().findRoom(u) == NULL) {
      vnum = u;
      break;
    }
  }

  return vnum;
}

void Area::reset(void) {
  for (std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it) {
    it->second->reset();
  }
  World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Area Reset: %s (%lu)\n", name().c_str(), ID());
  return;
}

unsigned Area::howManyMobs(unsigned long vnum) {
  unsigned number_of_mobs = 0;
  for (std::map<unsigned long,Room*>::iterator r_it = rooms().begin(); r_it != rooms().end(); ++r_it) {
    for (std::list<Creature*>::iterator c_it = r_it->second->creatures().begin(); c_it != r_it->second->creatures().end(); ++c_it) {
      if (*c_it && (*c_it)->isMob()) {
        if (((Mob*)(*c_it))->vnum() == vnum) {
          number_of_mobs++;
        }
      }
    }
  }
  return number_of_mobs;
}

void Area::setTerrain(const char* terrain_name) {
  terrain(TerrainTable::Instance().find(terrain_name));
  if (!terrain()) {
    terrain(TerrainTable::Instance().find("city"));
  }
  return;
}

std::string Area::serializeBuilders(void) const {
  return Regex::implode("|", builders());
}

void Area::unserializeBuilders(const std::string& serialization) {
  std::vector<std::string> names = Regex::explode("|", serialization);
  for (auto iter : names) builders().insert(iter);
  return;
}

bool Area::hasPermission(Avatar* avatar) const {
  // Only the Administrator can fiddle with Limbo...
  if (ID() == 1 && avatar->level() < Creature::CREATOR) {
    return false;
  }
  // The Administrator and the Head Builder can edit anything else...
  if (avatar->adminFlags().test(ADMIN_HEADBUILDER) || avatar->level() >= Creature::CREATOR) {
    return true;
  }
  // The average Joe needs explicit permission for a given area...
  if (builders().find(avatar->name()) != builders().end()) {
    return true;
  }
  return false;
}

void Area::grantPermission(Avatar* avatar) {
  builders().insert(avatar->name());
  return;
}

void Area::revokePermission(Avatar* avatar) {
  builders().erase(avatar->name());
  return;
}
