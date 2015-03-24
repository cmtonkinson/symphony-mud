
#include "exit.hpp"
#include "os.hpp"
#include "placement-item.hpp"
#include "placement-npc.hpp"
#include "room.hpp"
#include "storage.hpp"
#include "terrain-table.hpp"
#include "world.hpp"
#include "zone.hpp"

Zone::Zone(void) {
  return;
}

Zone::Zone(unsigned long low, unsigned long high) {
  name("Undefined");
  this->low(low);
  this->high(high);
  terrain(TerrainTable::Instance().find("city"));
}

Zone::~Zone(void) {
  return;
}

void Zone::name(const char* name) {
  ColorString cs(name);
  _name = cs.stripColor();
  return;
}

void Zone::initialize(void) {
  World::Instance().insert(this);
  World::Instance().schedule()->add(new RecurringJob(this, &Zone::reset, 300, 600));
  return;
}

void Zone::save(void) {
  std::string filename = "data/zones/" + Regex::slugify(name()) + ".zone.txt";
  FILE* fp = 0;

  if ((fp = fopen(filename.c_str(), "w")) != NULL) {
    Storage::write(fp, this);
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to write zone file %s.\n", filename.c_str());
  }

  return;
}

Zone* Zone::load(std::string filename) {
  Zone* zone = nullptr;
  FILE* fp   = nullptr;

  if ((fp = fopen(filename.c_str(), "r")) != NULL) {
    zone = new Zone();
    Storage::read(fp, zone);
    zone->initialize();
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to read zone file %s.\n", filename.c_str());
  }

  return zone;
}

bool Zone::destroy(void) {
  // Destroy rooms.
  while (rooms().size()) rooms().begin()->second->destroy();
  // Remove this from the World.
  World::Instance().remove(this);
  // Delete the zone file.
  std::string filename = "data/zones/" + Regex::slugify(name()) + ".zone.txt";
  remove(filename.c_str());
  // Self-delete.
  delete this;
  // Success!
  return true;
}

void Zone::insert(Room* room) {
  room->zone(this);
  _rooms.insert(std::make_pair(room->vnum(), room));
  return;
}

void Zone::insert(Item* item) {
  _items.insert(std::make_pair(item->vnum(), item));
  return;
}

void Zone::insert(Npc* npc) {
  _npcs.insert(std::make_pair(npc->vnum(), npc));
  return;
}

unsigned long Zone::lowestAvailableRoom(void) {
  unsigned long vnum = 0;

  for (unsigned long u = low(); u <= high(); ++u) {
    if (World::Instance().findRoom(u) == NULL) {
      vnum = u;
      break;
    }
  }

  return vnum;
}

void Zone::reset(void) {
  for (std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it) {
    it->second->reset();
  }
  VERBOSE_(0, "zone '%s' reset", name().c_str())
  return;
}

unsigned Zone::howManyNpcs(unsigned long vnum) {
  unsigned number_of_npcs = 0;
  for (std::map<unsigned long,Room*>::iterator r_it = rooms().begin(); r_it != rooms().end(); ++r_it) {
    for (std::list<Being*>::iterator c_it = r_it->second->beings().begin(); c_it != r_it->second->beings().end(); ++c_it) {
      if (*c_it && (*c_it)->isNpc()) {
        if (((Npc*)(*c_it))->vnum() == vnum) {
          number_of_npcs++;
        }
      }
    }
  }
  return number_of_npcs;
}

void Zone::setTerrain(const char* terrain_name) {
  terrain(TerrainTable::Instance().find(terrain_name));
  if (!terrain()) {
    terrain(TerrainTable::Instance().find("city"));
  }
  return;
}

std::string Zone::serializeBuilders(void) const {
  return Regex::implode("|", builders());
}

void Zone::unserializeBuilders(const std::string& serialization) {
  std::vector<std::string> names = Regex::explode("|", serialization);
  for (auto iter : names) builders().insert(iter);
  return;
}

bool Zone::hasPermission(Avatar* avatar) const {
  // Only the Administrator can fiddle with Limbo...
  if (ID() == 1 && avatar->level() < Being::CREATOR) {
    return false;
  }
  // The Administrator and the Head Builder can edit anything else...
  if (avatar->adminFlags().test(ADMIN_HEADBUILDER) || avatar->level() >= Being::CREATOR) {
    return true;
  }
  // The average Joe needs explicit permission for a given zone...
  if (builders().find(avatar->name()) != builders().end()) {
    return true;
  }
  return false;
}

void Zone::grantPermission(Avatar* avatar) {
  builders().insert(avatar->name());
  return;
}

void Zone::revokePermission(Avatar* avatar) {
  builders().erase(avatar->name());
  return;
}
