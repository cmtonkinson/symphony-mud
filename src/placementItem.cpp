
#include "commandTable-default.h"
#include "math.h"
#include "os.h"
#include "placementItem.h"
#include "room.h"
#include "world.h"

PlacementItem::PlacementItem(void) {
  type(ITEM);
  preposition(ERROR);
  indirectItem(ERROR);
  indirectItemIndex(ERROR);
  return;
}

PlacementItem::~PlacementItem(void) {
  return;
}

std::string PlacementItem::notes(void) const {
  std::string foo;

  switch (preposition()) {
    case ON:
      foo.assign("on item ").append(estring(indirectItem())).append("#").append(estring(indirectItemIndex()));
      break;
    case IN:
      foo.assign("in item ").append(estring(indirectItem())).append("#").append(estring(indirectItemIndex()));
      break;
    case CARRY:
      foo.assign("carried by npc ").append(estring(indirectItem())).append("#").append(estring(indirectItemIndex()));
      break;
    case WEAR:
      foo.assign("worn by npc ").append(estring(indirectItem())).append("#").append(estring(indirectItemIndex()));
      break;
    default:
      foo.assign("N/A");
      break;
  }

  return foo;
}

void PlacementItem::destroy(void) {
  delete this;
  return;
}

bool PlacementItem::execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs) {
  std::map<unsigned long,Item*>::iterator it;
  Zone* zone = room()->zone();
  Item* item = NULL;
  Being* being = NULL;
  unsigned already_there = room()->inventory().howManyItems(target());
  unsigned items_added = 0;
  CmdGet get;
  CmdWear wear;
  std::vector<std::string> args(1);

  if ((it = zone->items().find(target())) != zone->items().end()) {
    item = it->second;
  } else {
    WARN_(0, "placement failed in '%s'; item %lu doesn't exist", zone->name().c_str(), target())
    return false;
  }

  for (unsigned u = 0; u < number(); ++u) {
    if (already_there + items_added >= max()) break;
    if (Math::rand(1, 100) <= probability()) {
      item = new Item(*item);
      World::Instance().insert(item);
      room()->inventory().add(item);
      new_items.push_back(item);
      items_added++;
      SILLY_(0, "placement in '%s'; item %lu in room %lu", zone->name().c_str(), target(), room()->vnum())
      if (preposition() == CARRY || preposition() == WEAR) {
        being = room()->being_by_vnum(indirectItem(), indirectItemIndex());
        if (being == NULL) {
          WARN_(0, "placement failed in '%s'; item %lu loaded, but can't find NPC %lu#%hu", zone->name().c_str(), target(), indirectItem(), indirectItemIndex())
          return false;
        }
        args[0] = item->identifiers().longestKeyword();
        // Pick up the item.
        get.execute(being, args);
        // Wear it, if applicable.
        if (preposition() == WEAR) wear.execute(being, args);
      }
    }
  }

  return true;
}
