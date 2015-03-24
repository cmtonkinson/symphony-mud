
#include "placement.hpp"
#include "placement-npc.hpp"
#include "placement-item.hpp"
#include "world.hpp"

const unsigned Placement::ERROR;
const unsigned Placement::ITEM;
const unsigned Placement::NPC;
const unsigned Placement::ON;
const unsigned Placement::IN;
const unsigned Placement::CARRY;
const unsigned Placement::WEAR;

Placement::Placement(void) {
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

Placement::~Placement(void) {
  return;
}

Placement* Placement::create(const char* type) {
  if (strcmp(type, "ITEM") == 0) {
    return new PlacementItem();
  } else if (strcmp(type, "NPC") == 0) {
    return new PlacementNpc();
  } else {
    return nullptr;
  }
}
