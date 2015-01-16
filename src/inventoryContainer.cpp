/*
 * $Id: inventoryContainer.cpp 417 2010-06-26 22:24:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "estring.h"
#include "inventoryContainer.h"
#include "object.h"
#include "object-furniture.h"

InventoryContainer::InventoryContainer(std::string (Identifiers::*getName)(void) const):
    _getName(getName) {
  return;
}

InventoryContainer::InventoryContainer(const InventoryContainer& ref):
    _getName(ref._getName) {
  for (std::list<Object*>::const_iterator it = ref.objectList().begin(); it != ref.objectList().end(); ++it) {
    objectList().push_back(new Object(**it));
  }
  return;
}

InventoryContainer::~InventoryContainer(void) {
  purgeObjects();
  return;
}

void InventoryContainer::add(Object* object) {
  objectList().push_back(object);
  return;
}

void InventoryContainer::add(const std::list<Object*>& objects) {
  objectList().insert(objectList().end(), objects.begin(), objects.end());
  return;
}

void InventoryContainer::remove(Object* object) {
  objectList().remove(object);
  return;
}

void InventoryContainer::remove(const std::list<Object*>& objects) {
  for (std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
    remove(*it);
  }
  return;
}

void InventoryContainer::purgeObjects(void) {
  std::list<Object*> dead;
  for (std::list<Object*>::iterator it = objectList().begin(); it != objectList().end(); ++it) {
    // don't purge the following:
    //  -furniture with anyone on it
    if ((*it)->isFurniture() && (*it)->furniture()->current()) {
      continue;
    }
    dead.push_back(*it);
  }
  while (!dead.empty()) {
    delete dead.front();
    objectList().remove(dead.front());
    dead.pop_front();
  }
  return;
}

std::list<Object*> InventoryContainer::searchObjects(const std::string& q) {
  std::vector<std::string> keywords;
  int multiplier = 0;
  int index = 0;

  keywords = Container::parseQuery(q, multiplier, index);
  return Container::search(objectList(), keywords, multiplier, index);
}

std::list<Object*> InventoryContainer::searchObjects(const unsigned long& vnum) {
  std::list<Object*> l;
  for (std::list<Object*>::iterator it = objectList().begin(); it != objectList().end(); ++it) {
    if ((*it)->vnum() == vnum) {
      l.push_back(*it);
    }
  }
  return l;
}

Object* InventoryContainer::searchSingleObject(const std::string& q) {
  std::list<Object*> l = searchObjects(q);
  if (l.empty()) {
    return NULL;
  } else {
    return l.front();
  }
}

unsigned InventoryContainer::howManyObjects(const unsigned long& vnum) const {
  unsigned number_of_objects = 0;
  for (std::list<Object*>::const_iterator it = objectList().begin(); it != objectList().end(); ++it) {
    if ((*it)->vnum() == vnum) {
      number_of_objects++;
    }
  }
  return number_of_objects;
}

std::string InventoryContainer::listObjects(bool compact) const {
  std::map<std::string,unsigned> map;
  std::string dest;

  if (compact) {
    for (std::list<Object*>::const_iterator it = objectList().begin(); it != objectList().end(); ++it) {
      if (map.find(((*it)->identifiers().*_getName)()) == map.end()) {
        map[((*it)->identifiers().*_getName)()] = 1;
      } else {
        map[((*it)->identifiers().*_getName)()] += 1;
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
    for (std::list<Object*>::const_iterator it = objectList().begin(); it != objectList().end(); ++it) {
      dest.append(((*it)->identifiers().*_getName)()).append(1, '\n');
    }
  }
  return Regex::trim(dest);
}
