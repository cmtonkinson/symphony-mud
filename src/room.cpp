
#include "display.hpp"
#include "exit.hpp"
#include "os.hpp"
#include "placement.hpp"
#include "terrain-table.hpp"
#include "world.hpp"
#include "zone.hpp"

Room::Room(void): _inventory(&Identifiers::longname) {
  for (unsigned u = 0; u < 6; ++u) _exits[u] = nullptr;
  return;
}

Room::Room(unsigned long vnum, Zone* zone): _inventory(&Identifiers::longname) {
  this->zone(zone);
  zone->rooms().insert(std::make_pair(vnum, this));
  this->vnum(vnum);
  flags().value(0);
  name("Undefined");
  terrain(zone->terrain());
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
  while (!placements().empty()) {
    delete placements().front();
    placements().pop_front();
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

void Room::removePlacement(unsigned index) {
  std::list<Placement*>::iterator it = placements().begin();
  Placement* rule = NULL;
  unsigned short i = 0;
  if (index && index <= placements().size()) {
    for (i = 1; i <= placements().size(); ++i, ++it) {
      if (i == index) {
        rule = *it;
        placements().erase(it);
        rule->destroy();
        return;
      }
    }
  }
  return;
}

void Room::executePlacements(void) {
  std::list<Item*> new_items;
  std::list<Npc*> new_npcs;
  for (std::list<Placement*>::iterator it = placements().begin(); it != placements().end(); ++it) {
    (*it)->execute(new_items, new_npcs);
  }
  return;
}

void Room::send(std::string format, Being* being, const void* arg1, const void* arg2, unsigned long target) {
  std::string message;

  format = Regex::rtrim(format).append(1, '\n');
  for (std::list<Being*>::iterator c_it = beings().begin(); c_it != beings().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == TO_BEING  && *c_it != being)
          || (target == TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == TO_NOTVICT   && (*c_it == being || *c_it == arg1 || *c_it == arg2))
          || (target == TO_ROOM      && *c_it == being)
       ) {
      continue;
    }
    (*c_it)->send(Display::formatAction(format.c_str(), being, arg1, arg2, *c_it));
  }
  return;
}

void Room::send_cond(std::string format, Being* being, const void* arg1, const void* arg2, unsigned long target, bool audible) {
  std::string message;

  format = Regex::rtrim(format).append(1, '\n');
  for (std::list<Being*>::iterator c_it = beings().begin(); c_it != beings().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == TO_BEING  && *c_it != being)
          || (target == TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == TO_NOTVICT   && (*c_it == being || *c_it == arg1 || *c_it == arg2))
          || (target == TO_ROOM      && *c_it == being)
       ) {
      continue;
    }
    // Only send if the actor can be seen or heard...
    if ((*c_it)->canSee(being) > Being::SEE_NOTHING || audible) {
      (*c_it)->send(Display::formatAction(format.c_str(), being, arg1, arg2, *c_it));
    }
  }
  return;
}

void Room::indented_send(unsigned spaces, std::string format, Being* being, const void* arg1, const void* arg2, unsigned long target) {
  std::string padding;
  for (unsigned u = 0; u < spaces; ++u) padding.append(" ");
  send(padding + format, being, arg1, arg2, target);
  return;
}

void Room::indented_send_cond(unsigned spaces, std::string format, Being* being, const void* arg1, const void* arg2, unsigned long target, bool audible) {
  std::string padding;
  for (unsigned u = 0; u < spaces; ++u) padding.append(" ");
  send_cond(padding + format, being, arg1, arg2, target, audible);
  return;
}

void Room::destroy(void) {
  // Remove this Room from the parent Zone.
  zone()->rooms().erase(vnum());
  // Destroy outbound Exits.
  for (unsigned u = 0; u < 6; ++u) {
    if (exit(u)) {
      exit(u)->destroy();
      exit(u, NULL);
    }
  }
  // Destroy inbound exits (takes a bit more effort).
  for (std::set<Zone*,zone_comp>::iterator a = World::Instance().getZones().begin(); a != World::Instance().getZones().end(); ++a) {
    for (std::map<unsigned long,Room*>::iterator r = (*a)->rooms().begin(); r != (*a)->rooms().end(); ++r) {
      for (unsigned u = NORTH; u <= DOWN; ++u) {
        if (r->second->exit(u) && r->second->exit(u)->targetRoom() == this) {
          r->second->exit(u)->destroy();
          r->second->exit(u, NULL);
        }
      }
    }
  }
  // Destroy Placements.
  for (auto iter : placements()) iter->destroy();
  // Make sure the Room is empty first.
  if (!clear()) {
    ERROR_(nullptr, "Couldn't clear() Room #%lu during destruction.\n", vnum());
    return; // shouldn't delete if it couldn't be clear()ed
  }
  // Self-delete
  delete this;
  return;
}

// Uses a wormhole to clear all Beings and Items from the Room,
// moving them to the Tundra.
bool Room::clear(void) {
  // Clear Beings...
  while (beings().size()) {
    if (!World::Instance().transport(*(beings().begin()), 0)) {
      return false;
    }
  }
  // Clear Items...
  while (inventory().itemList().size()) {
    if (!World::Instance().transport(*(inventory().itemList().begin()), this, 0)) {
      return false;
    }
  }
  return true;
}

void Room::reset(void) {
  std::list<Item*> new_items;
  std::list<Npc*> new_npcs;
  for (std::list<Placement*>::iterator it = placements().begin(); it != placements().end(); ++it) {
    (*it)->execute(new_items, new_npcs);
  }
  return;
}

Being* Room::being_by_vnum(unsigned long vnum, unsigned index) {
  std::list<Being*>::iterator iter;
  std::list<Being*> matches;
  // Find all vnum matches.
  for (iter = beings().begin(); iter != beings().end(); ++iter) {
    if (!(*iter)->isNpc()) continue;
    if (((Npc*)(*iter))->vnum() == vnum) matches.push_back(*iter);
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
  sprintf(buffer, "zone..... {y%s{x\n\
vnum..... {y%lu{x\n\
name..... {y%s{x\n\
flags.... {y%s{x\n\
terrain.. {y%s{x\n\
smell.... {y%s{x\n\
sound.... {y%s{x\n\n\
  --== {Y description{x ==--\n%s\n\
",  room->zone()->name().c_str(), room->vnum(), room->name().c_str(), room->flags().list(FTRoom::Instance()).c_str(),
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
