
#include "math.h"
#include "os.h"
#include "placementNpc.h"
#include "room.h"
#include "world.h"

PlacementNpc::PlacementNpc(void) {
  type(NPC);
  return;
}

PlacementNpc::~PlacementNpc(void) {
  return;
}

std::string PlacementNpc::notes(void) const {
  return std::string();
}

void PlacementNpc::destroy(void) {
  delete this;
  return;
}

bool PlacementNpc::execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs) {
  unsigned npcs_added = 0;
  int level           = 0;
  std::map<unsigned long,Npc*>::iterator it;
  Zone* zone = room()->zone();
  Npc* npc = NULL;
  unsigned already_there = zone->howManyNpcs(target());

  if ((it = zone->npcs().find(target())) != zone->npcs().end()) {
    npc = it->second;
  } else {
    WARN_(0, "placement failed in '%s'; NPC %lu doesn't exist", zone->name().c_str(), target())
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + npcs_added >= max()) break;
    if (Math::rand(1, 100) <= probability()) {
      npc = new Npc(*npc);
      World::Instance().insert(npc);
      npc->room(room());
      room()->add(npc);
      room()->send_cond("$p has arrived.\n", npc);
      new_npcs.push_back(npc);
      npcs_added++;
      SILLY_(0, "placement in '%s'; NPC %lu in room %lu", zone->name().c_str(), target(), room()->vnum())
      // Give the Npc some real stats. It's a hack to forcibly level them just to make the numbers
      // come out, but it works for now and it has the added benefit of making existing Npcs' stats
      // update to reflect modifications to the growth algorithms during active development.
      level = npc->level();
      npc->resetStats();
      for (int i = 1; i < level; ++i) npc->gainLevel();
      // Imbibe the npc with some life, as appropriate.
      npc->npcilize();
    }
  }

  return true;
}
