
#include "area.h"
#include "display.h"
#include "exit.h"
#include "loadRule.h"
#include "terrainTable.h"
#include "world.h"

Room::Room(void): _inventory(&Identifiers::longname) {
  for (unsigned u = 0; u < 6; ++u) _exits[u] = nullptr;
  return;
}

Room::Room(unsigned long vnum, Area* area): _inventory(&Identifiers::longname) {
  try {
    char query[Socket::MAX_BUFFER];

    sprintf(query, "INSERT IGNORE INTO rooms (areaID, vnum) VALUES (%lu, %lu);", area->ID(), vnum);
    World::Instance().getMysql()->insert(query);
    ID(World::Instance().getMysql()->getInsertID());
  } catch (MysqlException me) {
    fprintf(stderr, "Failed to create room for area %lu: %s\n", area->ID(), me.getMessage().c_str());
    return;
  }

  this->area(area);
  area->rooms().insert(std::make_pair(vnum, this));
  this->vnum(vnum);
  flags().value(0);
  name("Undefined");
  terrain(area->terrain());
  for (unsigned u = 0; u < 6; ++u) {
    _exits[u] = nullptr;
  }

  return;
}

Room::Room(Area* area, ROW& row): _inventory(&Identifiers::longname) {
  this->area(area);
  ID(row["roomID"]);
  vnum(row["vnum"]);
  name(row["name"]);
  description(row["description"]);
  smell(row["smell"]);
  sound(row["sound"]);
  terrain(TerrainTable::Instance().find(row["terrain"]));
  if (!terrain()) {
    terrain(TerrainTable::Instance().find("city"));
  }
  flags().value(row["flags"]);
  for (unsigned u = 0; u < 6; ++u) {
    _exits[u] = nullptr;
  }
  return;
}

Room::~Room(void) {
  for (unsigned u = 0; u < 6; ++u) {
    if (_exits[u]) {
      delete _exits[u];
    }
  }
  while (!loadRules().empty()) {
    delete loadRules().front();
    loadRules().pop_front();
  }
  return;
}

void Room::exit(unsigned direction, Exit* exit) {
  if (NORTH <= direction && direction <= DOWN) _exits[direction-1] = exit;
  return;
}

Exit* Room::exit(unsigned direction) {
  if (NORTH <= direction && direction <= DOWN) return _exits[direction-1];
  return nullptr;
}

Exit* Room::exit(std::string direction) {
  return exit(Exit::string2dir(direction));
}

void Room::removeLoadRule(unsigned index) {
  std::list<LoadRule*>::iterator it = loadRules().begin();
  LoadRule* rule = NULL;
  unsigned short i = 0;
  if (index && index <= loadRules().size()) {
    for (i = 1; i <= loadRules().size(); ++i, ++it) {
      if (i == index) {
        rule = *it;
        loadRules().erase(it);
        rule->destroy();
        return;
      }
    }
  }
  return;
}

void Room::executeLoadRules(void) {
  std::list<Object*> new_objects;
  std::list<Mob*> new_mobs;
  for (std::list<LoadRule*>::iterator it = loadRules().begin(); it != loadRules().end(); ++it) {
    (*it)->execute(new_objects, new_mobs);
  }
  return;
}

void Room::send(std::string format, Creature* creature, void* arg1, void* arg2, unsigned long target) {
  std::string message;

  format = Regex::trim(format).append(1, '\n');
  for (std::list<Creature*>::iterator c_it = creatures().begin(); c_it != creatures().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == TO_CREATURE  && *c_it != creature)
          || (target == TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == TO_NOTVICT   && (*c_it == creature || *c_it == arg1 || *c_it == arg2))
          || (target == TO_ROOM      && *c_it == creature)
       ) {
      continue;
    }
    (*c_it)->send(Display::formatAction(format.c_str(), creature, arg1, arg2, *c_it));
  }
  return;
}

void Room::send_cond(std::string format, Creature* creature, void* arg1, void* arg2, unsigned long target, bool audible) {
  std::string message;

  format = Regex::trim(format).append(1, '\n');
  for (std::list<Creature*>::iterator c_it = creatures().begin(); c_it != creatures().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == TO_CREATURE  && *c_it != creature)
          || (target == TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == TO_NOTVICT   && (*c_it == creature || *c_it == arg1 || *c_it == arg2))
          || (target == TO_ROOM      && *c_it == creature)
       ) {
      continue;
    }
    // Only send if the actor can be seen or heard...
    if ((*c_it)->canSee(creature) > Creature::SEE_NOTHING || audible) {
      (*c_it)->send(Display::formatAction(format.c_str(), creature, arg1, arg2, *c_it));
    }
  }
  return;
}

void Room::save(void) {
  try {
    char query[Socket::MAX_BUFFER];

    sprintf(query,
      "UPDATE rooms SET     \
        name = '%s',        \
        description = '%s', \
        smell = '%s',       \
        sound = '%s',       \
        terrain = '%s',     \
        flags = %lu         \
       WHERE roomID = %lu   \
       LIMIT 1;",
      Mysql::addslashes(name()).c_str(),
      Mysql::addslashes(description()).c_str(),
      Mysql::addslashes(smell()).c_str(),
      Mysql::addslashes(sound()).c_str(),
      Mysql::addslashes(terrain()->name()).c_str(),
      flags().value(),
      ID()
   );
    World::Instance().getMysql()->update(query);

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to save room %lu: %s\n", ID(), me.getMessage().c_str());
    return;
  }

  // Save exits...
  for (unsigned u = 0; u < 6; ++u) {
    if (exit(u)) {
      exit(u)->save();
    }
  }

  return;
}

bool Room::destroy(void) {
  unsigned long tempID = ID();

  // Get everything out of the room before we annihilate it...
  if (!clear()) {
    return false;
  }

  try {
    char query[Socket::MAX_BUFFER];

    // Get rid of this (from the DB)...
    sprintf(query,
      " DELETE              \
        FROM rooms          \
        WHERE roomID = %lu  \
        LIMIT 1;",
      ID()
   );
    World::Instance().getMysql()->remove(query);
    // Get rid of this (from parent area)...
    area()->rooms().erase(vnum());
    // Get rid of outbound exits...
    for (unsigned u = 0; u < 6; ++u) {
      if (exit(u)) {
        exit(u)->destroy();
        exit(u, NULL);
      }
    }
    // Get rid of inbound exits (this takes a bit more effort)...
    for (std::set<Area*,area_comp>::iterator a = World::Instance().getAreas().begin(); a != World::Instance().getAreas().end(); ++a) {
      for (std::map<unsigned long,Room*>::iterator r = (*a)->rooms().begin(); r != (*a)->rooms().end(); ++r) {
        for (unsigned u = NORTH; u <= DOWN; ++u) {
          if (r->second->exit(u) && r->second->exit(u)->targetRoom() == this) {
            r->second->exit(u)->destroy();
            r->second->exit(u, NULL);
          }
        }
      }
    }
    // Get rid of LoadRules for this room...
    sprintf(query,
      " DELETE              \
        FROM load_rules     \
        WHERE vnum = %lu    \
        ;",
      vnum()
   );
    World::Instance().getMysql()->remove(query);
    // Get rid of this (from memory)...
    delete this;

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to delete room %lu: %s\n", tempID, me.getMessage().c_str());
    return false;
  }

  // Success!
  return true;
}

bool Room::clear(void) {
  // Clear Creatures...
  while (creatures().size()) {
    if (!World::Instance().transport(*(creatures().begin()), 0)) {
      return false;
    }
  }
  // Clear Objects...
  while (inventory().objectList().size()) {
    if (!World::Instance().transport(*(inventory().objectList().begin()), this, 0)) {
      return false;
    }
  }
  return true;
}

void Room::reset(void) {
  std::list<Object*> new_objects;
  std::list<Mob*> new_mobs;
  for (std::list<LoadRule*>::iterator it = loadRules().begin(); it != loadRules().end(); ++it) {
    (*it)->execute(new_objects, new_mobs);
  }
  return;
}

Creature* Room::creature_by_vnum(unsigned long vnum, unsigned index) {
  std::list<Creature*>::iterator iter;
  std::list<Creature*> matches;
  // Find all vnum matches.
  for (iter = creatures().begin(); iter != creatures().end(); ++iter) {
    if (!(*iter)->isMob()) continue;
    if (((Mob*)(*iter))->vnum() == vnum) matches.push_back(*iter);
  }
  if (matches.empty()) return NULL;
  if (index > matches.size()) return NULL;
  if (index == INDEX_DEFAULT) return matches.front();
  for (int i = 1; i < index; ++i) matches.pop_front();
  return matches.front();
}

void Room::setTerrain(const char* terrain_name) {
  terrain(TerrainTable::Instance().find(terrain_name));
  if (!terrain()) {
    terrain(TerrainTable::Instance().find("city"));
  }
  return;
}

std::string Room::getInformation(Room* room) {
  std::string output;
  char buffer[Socket::MAX_BUFFER];
  char exit_buf[Socket::MAX_BUFFER];

  output.append(" --== {Yroom data{x ==--\n");
  sprintf(buffer, "area..... {y%s{x\n\
vnum..... {y%lu{x\n\
name..... {y%s{x\n\
flags.... {y%s{x\n\
terrain.. {y%s{x\n\
smell.... {y%s{x\n\
sound.... {y%s{x\n\n\
  --== {Y description{x ==--\n%s\n\
",  room->area()->name().c_str(), room->vnum(), room->name().c_str(), room->flags().list(FTRoom::Instance()).c_str(),
    room->terrain()->name().c_str(), room->smell().c_str(), room->sound().c_str(), room->description().c_str());
  output.append(buffer);

  output.append(" --== {Yexit data{x ==--\n");
  for (unsigned u = NORTH; u <= DOWN; ++u) {
    if (room->exit(u)) {
      sprintf(exit_buf, "{y%s{x to room {y%lu{x ({y%s{x)\n", Exit::name(u), room->exit(u)->targetRoom()->vnum(), room->exit(u)->targetRoom()->name().c_str());
      output.append(exit_buf);
      sprintf(exit_buf, "  Flags: {y%s{x\n", room->exit(u)->flags().list(FTRoom::Instance()).c_str());
      output.append(exit_buf);
      if (room->exit(u)->key()) {
        sprintf(exit_buf, "  Key: {y%lu{x\n", room->exit(u)->key());
        output.append(exit_buf);
      }
    }
  }

  return output;
}
