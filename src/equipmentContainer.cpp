
#include "being.h"
#include "equipmentContainer.h"
#include "object.h"

EquipmentContainer::EquipmentContainer(void) {
  return;
}

EquipmentContainer::EquipmentContainer(const EquipmentContainer& ref) {
  for (std::map<int,Object*>::const_iterator it = ref.objectMap().begin(); it != ref.objectMap().end(); ++it) {
    objectMap()[it->first] = new Object(*(it->second));
  }
  return;
}

EquipmentContainer::~EquipmentContainer(void) {
  purgeObjects();
  return;
}

void EquipmentContainer::add(Object* object, const int& location) {
  objectMap()[location] = object;
  return;
}

void EquipmentContainer::remove(Object* object) {
  for (std::map<int,Object*>::iterator it = objectMap().begin(); it != objectMap().end(); ++it) {
    if (it->second == object) {
      objectMap().erase(it);
      break;
    }
  }
  return;
}

void EquipmentContainer::remove(const std::list<Object*>& objects) {
  for (std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
    remove(*it);
  }
  return;
}

void EquipmentContainer::purgeObjects(void) {
  for (std::map<int,Object*>::iterator it = objectMap().begin(); it != objectMap().end(); ++it) {
    if (it->second) {
      delete it->second;
    }
  }
  objectMap().clear();
  return;
}

std::list<Object*> EquipmentContainer::searchObjects(const std::string& q) {
  std::vector<std::string> keywords;
  int multiplier = 0;
  int index = 0;
  keywords = Container::parseQuery(q, multiplier, index);
  return Container::search(objectMap(), keywords, multiplier, index);
}

Object* EquipmentContainer::searchSingleObject(const std::string& q) {
  std::list<Object*> l = searchObjects(q);
  if (l.empty()) {
    return NULL;
  } else {
    return l.front();
  }
}

std::string EquipmentContainer::listObjects(void) const {
  std::map<std::string,unsigned> map;
  std::string dest;
  for (std::map<int,Object*>::const_iterator it = objectMap().begin(); it != objectMap().end(); ++it) {
    dest.append(Being::wearLocName(it->first));
    dest.append(it->second->decorativeShortname());
    dest.append("\n");
  }
  return Regex::trim(dest);
}

Object* EquipmentContainer::at(int location) {
  std::map<int,Object*>::iterator iter = objectMap().find(location);
  if (iter != objectMap().end()) return iter->second;
  return NULL;
}
