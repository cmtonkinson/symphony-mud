
#include "loadRule.h"
#include "world.h"

LoadRule::LoadRule(void) {
  ID(0);
  vnum(0);
  room(NULL);
  type(0);
  target(0);
  number(1);
  max(1);
  probability(100);
  return;
}

LoadRule::LoadRule(ROW row) {
  ID(row["id"]);
  vnum(row["vnum"]);
  room(World::Instance().findRoom(vnum()));
  if (row["type"] == "MOB") {
    type(MOB);
  } else if (row["type"] == "OBJECT") {
    type(OBJECT);
  }
  target(row["target"]);
  number(row["number"]);
  max(row["max"]);
  probability(row["probability"]);
  return;
}

LoadRule::~LoadRule(void) {
  return;
}

bool LoadRule::destroy(void) {
  unsigned long tempID = ID();

  try {
    char query[Socket::MAX_BUFFER];

    sprintf(query,
      " DELETE              \
        FROM load_rules     \
        WHERE id = %lu      \
        LIMIT 1;",
      tempID
   );
    // Get rid of this (from the DB)...
    World::Instance().getMysql()->remove(query);
    // Get rid of this (from memory)...
    delete this;

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to delete load_rule %lu: %s\n", tempID, me.getMessage().c_str());
    return false;
  };

  // Success!
  return true;
}

