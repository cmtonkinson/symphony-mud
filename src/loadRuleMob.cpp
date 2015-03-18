
#include "loadRuleMob.h"
#include "room.h"
#include "world.h"

LoadRuleMob::LoadRuleMob(void) {
  type(MOB);
  return;
}

LoadRuleMob::~LoadRuleMob(void) {
  return;
}

std::string LoadRuleMob::notes(void) const {
  return std::string();
}

void LoadRuleMob::destroy(void) {
  delete this;
  return;
}

bool LoadRuleMob::execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs) {
  unsigned mobs_added = 0;
  int level           = 0;
  std::map<unsigned long,Mob*>::iterator it;
  Area* area = room()->area();
  Mob* mob = NULL;
  unsigned already_there = area->howManyMobs(target());

  if ((it = area->mobs().find(target())) != area->mobs().end()) {
    mob = it->second;
  } else {
    World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset a mob. Mob %lu in area %lu doesn't exist.", target(), area->ID());
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + mobs_added >= max()) break;
    if (World::rand(1, 100) <= probability()) {
      mob = new Mob(*mob);
      World::Instance().insert(mob);
      mob->room(room());
      room()->add(mob);
      room()->send_cond("$p has arrived.\n", mob);
      new_mobs.push_back(mob);
      mobs_added++;
      // Give the Mob some real stats. It's a hack to forcibly level them just to make the numbers
      // come out, but it works for now and it has the added benefit of making existing Mobs' stats
      // update to reflect modifications to the growth algorithms during active development.
      level = mob->level();
      mob->resetStats();
      for (int i = 1; i < level; ++i) mob->gainLevel();
      // Imbibe the mob with some life, as appropriate.
      mob->mobilize();
    }
  }

  return true;
}
