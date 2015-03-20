
#include "loadRuleNpc.h"
#include "room.h"
#include "world.h"

LoadRuleNpc::LoadRuleNpc(void) {
  type(NPC);
  return;
}

LoadRuleNpc::~LoadRuleNpc(void) {
  return;
}

std::string LoadRuleNpc::notes(void) const {
  return std::string();
}

void LoadRuleNpc::destroy(void) {
  delete this;
  return;
}

bool LoadRuleNpc::execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs) {
  unsigned npcs_added = 0;
  int level           = 0;
  std::map<unsigned long,Npc*>::iterator it;
  Zone* zone = room()->zone();
  Npc* npc = NULL;
  unsigned already_there = zone->howManyNpcs(target());

  if ((it = zone->npcs().find(target())) != zone->npcs().end()) {
    npc = it->second;
  } else {
    World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset a npc. Npc %lu in zone %lu doesn't exist.", target(), zone->ID());
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + npcs_added >= max()) break;
    if (World::rand(1, 100) <= probability()) {
      npc = new Npc(*npc);
      World::Instance().insert(npc);
      npc->room(room());
      room()->add(npc);
      room()->send_cond("$p has arrived.\n", npc);
      new_npcs.push_back(npc);
      npcs_added++;
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
