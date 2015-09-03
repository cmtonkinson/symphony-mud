
#include "estring.hpp"
#include "inventory-container.hpp"
#include "item-types.hpp"
#include "item.hpp"
#include "regex.hpp"

InventoryContainer::InventoryContainer(std::string (HasIdentifiers::*getName)(void) const):
    _getName(getName) {
  return;
}

InventoryContainer::InventoryContainer(const InventoryContainer& ref):
    _getName(ref._getName) {
  for (std::list<Item*>::const_iterator it = ref.itemList().begin(); it != ref.itemList().end(); ++it) {
    itemList().push_back(new Item(**it));
  }
  return;
}

InventoryContainer::~InventoryContainer(void) {
  purgeItems();
  return;
}

void InventoryContainer::add(Item* item) {
  itemList().push_back(item);
  return;
}

void InventoryContainer::add(const std::list<Item*>& items) {
  itemList().insert(itemList().end(), items.begin(), items.end());
  return;
}

void InventoryContainer::remove(Item* item) {
  itemList().remove(item);
  return;
}

void InventoryContainer::remove(const std::list<Item*>& items) {
  for (std::list<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
    remove(*it);
  }
  return;
}

void InventoryContainer::purgeItems(void) {
  std::list<Item*> dead;
  for (std::list<Item*>::iterator it = itemList().begin(); it != itemList().end(); ++it) {
    // don't purge the following:
    //  -furniture with anyone on it
    if ((*it)->isFurniture() && (*it)->furniture()->current()) {
      continue;
    }
    dead.push_back(*it);
  }
  while (!dead.empty()) {
    delete dead.front();
    itemList().remove(dead.front());
    dead.pop_front();
  }
  return;
}

std::list<Item*> InventoryContainer::searchItems(const std::string& q) {
  std::vector<std::string> keywords;
  int multiplier = 0;
  int index = 0;

  keywords = Container::parseQuery(q, multiplier, index);
  return Container::search(itemList(), keywords, multiplier, index);
}

std::list<Item*> InventoryContainer::searchItems(const unsigned long& vnum) {
  std::list<Item*> l;
  for (std::list<Item*>::iterator it = itemList().begin(); it != itemList().end(); ++it) {
    if ((*it)->vnum() == vnum) {
      l.push_back(*it);
    }
  }
  return l;
}

Item* InventoryContainer::searchSingleItem(const std::string& q) {
  std::list<Item*> l = searchItems(q);
  if (l.empty()) {
    return NULL;
  } else {
    return l.front();
  }
}

unsigned InventoryContainer::howManyItems(const unsigned long& vnum) const {
  unsigned number_of_items = 0;
  for (std::list<Item*>::const_iterator it = itemList().begin(); it != itemList().end(); ++it) {
    if ((*it)->vnum() == vnum) {
      number_of_items++;
    }
  }
  return number_of_items;
}

std::string InventoryContainer::listItems(bool compact) const {
  std::map<std::string,unsigned> map;
  std::string dest;

  if (compact) {
    for (std::list<Item*>::const_iterator it = itemList().begin(); it != itemList().end(); ++it) {
      if (map.find(((*it)->*_getName)()) == map.end()) {
        map[((*it)->*_getName)()] = 1;
      } else {
        map[((*it)->*_getName)()] += 1;
      }
    }
    for (std::map<std::string,unsigned>::iterator it = map.begin(); it != map.end(); ++it) {
      if (it->second > 1) {
        dest.append(1, '(').append(estring(it->second)).append(") ").append(it->first).append(1, '\n');
      } else {
        dest.append(it->first).append(1, '\n');
      }
    }
  } else {
    for (std::list<Item*>::const_iterator it = itemList().begin(); it != itemList().end(); ++it) {
      dest.append(((*it)->*_getName)()).append(1, '\n');
    }
  }
  return Regex::trim(dest);
}
