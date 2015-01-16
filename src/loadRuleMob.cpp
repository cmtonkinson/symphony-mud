
#include "loadRuleMob.h"
#include "room.h"
#include "world.h"

LoadRuleMob::LoadRuleMob(void) {
  return;
}

LoadRuleMob::LoadRuleMob(ROW row): LoadRule(row) {
  return;
}

LoadRuleMob::~LoadRuleMob(void) {
  return;
}

std::string LoadRuleMob::notes(void) const {
  return std::string();
}

bool LoadRuleMob::save(void) {
  return false;
}

bool LoadRuleMob::commit(void) {
  try {
    char query[Socket::MAX_BUFFER];

    sprintf(
      query,
      "INSERT IGNORE INTO load_rules (vnum, type, target, number, max, probability) VALUES (%lu, '%s', %lu, %u, %u, %u);",
      vnum(),
      (type() == MOB ? "MOB" : "OBJECT"),
      target(),
      number(),
      max(),
      probability()
   );
    World::Instance().getMysql()->insert(query);
    ID(World::Instance().getMysql()->getInsertID());

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to commit load_rule vnum %lu: %s\n", vnum(), me.getMessage().c_str());
    return false;
  }

  return true;
}

bool LoadRuleMob::execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs) {
  std::map<unsigned long,Mob*>::iterator it;
  Area* area = room()->area();
  Mob* mob = NULL;
  unsigned already_there = area->howManyMobs(target());
  unsigned mobs_added = 0;
  int level = 0;

  if ((it = area->mobs().find(target())) != area->mobs().end()) {
    mob = it->second;
  } else {
    World::Instance().bigBrother(NULL, ADMIN_BIGBRO_RESETS, "Failed to reset a mob.  Mob %lu in area %lu doesn't exist.", target(), area->ID());
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + mobs_added >= max()) {
      break;
    }
    if (World::rand(1, 100) <= probability()) {
      mob = new Mob(*mob);
      World::Instance().insert(mob);
      mob->room(room());
      room()->add(mob);
      new_mobs.push_back(mob);
      mobs_added++;
      // Give the mob some real stats. It's a hack but it works for now.
      level = mob->level();
      mob->resetStats();
      for (int i = 1; i < level; ++i) mob->gainLevel();
    }
  }

  return true;
}
