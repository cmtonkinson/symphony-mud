/*
 * $Id: equipmentContainer.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_EQUIPMENT_CONTAINER
#define H_SYMPHONY_EQUIPMENT_CONTAINER

#include <list>
#include <map>
#include <string>
#include "container.h"
#include "identifiers.h"

class Object;

class EquipmentContainer: public Container {
  public:
    // constructors...
    EquipmentContainer(void);
    EquipmentContainer(const EquipmentContainer& ref);
    virtual ~EquipmentContainer(void);

    // virtual methods....
    virtual void                add(Object* object, const int& location);
    virtual void                remove(Object* object);
    virtual void                remove(const std::list<Object*>& objects);
    virtual void                purgeObjects(void);
    virtual std::list<Object*>  searchObjects(const std::string& q);
    virtual Object*             searchSingleObject(const std::string& q);

    // general methods...
    std::string                 listObjects(void) const;
    Object*                     at(int location);

    // public accessor methods...
    std::map<int,Object*>&                                  objectMap(void)       { return _objectMap; }
    const std::map<int,Object*>&                            objectMap(void) const { return _objectMap; }

  private:
    std::map<int,Object*> _objectMap;
};

#endif // !H_SYMPHONY_CONTAINER_MAP
