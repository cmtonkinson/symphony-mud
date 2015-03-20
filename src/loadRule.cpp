
#include "loadRule.h"
#include "loadRuleNpc.h"
#include "loadRuleItem.h"
#include "world.h"

const unsigned LoadRule::ERROR;
const unsigned LoadRule::ITEM;
const unsigned LoadRule::NPC;
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
  if (strcmp(type, "ITEM") == 0) {
    return new LoadRuleItem();
  } else if (strcmp(type, "NPC") == 0) {
    return new LoadRuleNpc();
  } else {
    return nullptr;
  }
}
