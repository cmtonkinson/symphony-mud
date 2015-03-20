
#include "being.h"
#include "equipmentContainer.h"
#include "item.h"

EquipmentContainer::EquipmentContainer(void) {
  return;
}

EquipmentContainer::EquipmentContainer(const EquipmentContainer& ref) {
  for (std::map<int,Item*>::const_iterator it = ref.itemMap().begin(); it != ref.itemMap().end(); ++it) {
    itemMap()[it->first] = new Item(*(it->second));
  }
  return;
}

EquipmentContainer::~EquipmentContainer(void) {
  purgeItems();
  return;
}

void EquipmentContainer::add(Item* item, const int& location) {
  itemMap()[location] = item;
  return;
}

void EquipmentContainer::remove(Item* item) {
  for (std::map<int,Item*>::iterator it = itemMap().begin(); it != itemMap().end(); ++it) {
    if (it->second == item) {
      itemMap().erase(it);
      break;
    }
  }
  return;
}

void EquipmentContainer::remove(const std::list<Item*>& items) {
  for (std::list<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
    remove(*it);
  }
  return;
}

void EquipmentContainer::purgeItems(void) {
  for (std::map<int,Item*>::iterator it = itemMap().begin(); it != itemMap().end(); ++it) {
    if (it->second) {
      delete it->second;
    }
  }
  itemMap().clear();
  return;
}

std::list<Item*> EquipmentContainer::searchItems(const std::string& q) {
  std::vector<std::string> keywords;
  int multiplier = 0;
  int index = 0;
  keywords = Container::parseQuery(q, multiplier, index);
  return Container::search(itemMap(), keywords, multiplier, index);
}

Item* EquipmentContainer::searchSingleItem(const std::string& q) {
  std::list<Item*> l = searchItems(q);
  if (l.empty()) {
    return NULL;
  } else {
    return l.front();
  }
}

std::string EquipmentContainer::listItems(void) const {
  std::map<std::string,unsigned> map;
  std::string dest;
  for (std::map<int,Item*>::const_iterator it = itemMap().begin(); it != itemMap().end(); ++it) {
    dest.append(Being::wearLocName(it->first));
    dest.append(it->second->decorativeShortname());
    dest.append("\n");
  }
  return Regex::trim(dest);
}

Item* EquipmentContainer::at(int location) {
  std::map<int,Item*>::iterator iter = itemMap().find(location);
  if (iter != itemMap().end()) return iter->second;
  return NULL;
}
