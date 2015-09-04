
#include <vector>
#include "being.hpp"
#include "has-modifiers.hpp"
#include "item-set.hpp"
#include "modifier.hpp"
#include "os.hpp"
#include "regex.hpp"
#include "world.hpp"
#include "zone.hpp"

ItemSet::ItemSet(void): HasModifiers() {
  return;
}

ItemSet::~ItemSet(void) {
  return;
}

std::string ItemSet::serializeItems(void) const {
  std::vector<std::string> foo;
  for (auto iter : items()) foo.push_back(estring(iter->vnum()));
  return Regex::implode("~", foo);
}

void ItemSet::unserializeItems(std::string ser) {
  std::vector<std::string> vnums = Regex::explode("~", ser);
  std::map<unsigned long,Item*>::iterator found;
  unsigned long vnum             = 0;
  Zone* zone                     = nullptr;

  for (auto iter : vnums) {
    vnum = estring(iter);
    if ((zone = World::Instance().lookup(vnum)) == nullptr) {
      ERROR_(nullptr, "Zone lookup failed for vnum %d while unserializing ItemSet %s.", vnum, ser.c_str());
      return;
    }
    if ((found = zone->items().find(vnum)) == zone->items().end()) {
      ERROR_(nullptr, "Item (vnum %d) lookup failed in Zone %s while unserializing ItemSet %s.", vnum, zone->ident(), ser.c_str());
      return;
    }
    World::Instance().addItemToSet(found->second, this);
  }
  return;
}

unsigned ItemSet::size(void) const {
  return items().size();
}

unsigned ItemSet::completion(Being* being) const {
  unsigned num = 0;
  for (auto iter : items()) {
    if (being->equipment().contains(iter->vnum())) {
      ++num;
    }
  }
  return num;
}

bool ItemSet::isComplete(Being* being) const {
  return completion(being) == size();
}

std::list<Item*> ItemSet::equipped(Being* being) const {
  std::list<Item*> eq;
  for (auto iter : items()) {
    if (being->equipment().contains(iter->vnum())) {
      eq.push_back(iter);
    }
  }
  return eq;
}

std::list<Item*> ItemSet::unequipped(Being* being) const {
  std::list<Item*> eq;
  for (auto iter : items()) {
    if (!being->equipment().contains(iter->vnum())) {
      eq.push_back(iter);
    }
  }
  return eq;
}

bool ItemSet::contains(Item* item) const {
  for (auto iter : items()) {
    if (iter->vnum() == item->vnum()) {
      return true;
    }
  }
  return false;
}

bool ItemSet::willCompleteSet(Being* being, Item* item) const {
  std::list<Item*> left;
  left.push_back(item);
  return unequipped(being) == left;
}

bool ItemSet::willBreakSet(Being* being, Item* item) const {
  return contains(item) && size() == completion(being);
}
