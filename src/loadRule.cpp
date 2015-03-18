
#include "loadRule.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "world.h"

const unsigned LoadRule::ERROR;
const unsigned LoadRule::OBJECT;
const unsigned LoadRule::MOB;
const unsigned LoadRule::ON;
const unsigned LoadRule::IN;
const unsigned LoadRule::CARRY;
const unsigned LoadRule::WEAR;

LoadRule::LoadRule(void) {
  ID(ERROR);
  vnum(ERROR);
  room(NULL);
  type(ERROR);
  target(ERROR);
  number(1);
  max(1);
  probability(100);
  return;
}

LoadRule::~LoadRule(void) {
  return;
}

LoadRule* LoadRule::create(const char* type) {
  if (strcmp(type, "OBJ") == 0) {
    return new LoadRuleObject();
  } else if (strcmp(type, "MOB") == 0) {
    return new LoadRuleMob();
  } else {
    return nullptr;
  }
}
