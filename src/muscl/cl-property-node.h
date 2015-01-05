/*
 * $Id: cl-property-node.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_CL_PROPERTY_NODE
#define SYMPHONY_CL_PROPERTY_NODE

#include "cl-node.h"

#include "datum.h"

template <class ObjectType, class DataType>
class PropertyNode: public ContextNode {
  public:
    PropertyNode(ObjectType* target, DataType (ObjectType::*getter)(void)const, void (ObjectType::*setter)(const DataType& ref)):
        ContextNode(~0),
        _target(target),
        _getter(getter),
        _setter(setter) {
      if (!_getter) _permission &= ~CL_PERM_READ;
      if (!_setter) _permission &= ~CL_PERM_WRITE;
      return;
    }

    virtual ~PropertyNode(void) { }

    virtual bool r(Datum& value) {
      if (readable()) {
        value = (_target->*_getter)();
        return true;
      }
      return false;
    }

    virtual bool w(const Datum& value) {
      if (writable()) {
        (_target->*_setter)((DataType)value);
        return true;
      }
      return false;
    }

  private:
    ObjectType* _target;
    DataType (ObjectType::*_getter)(void)const;
    void (ObjectType::*_setter)(const DataType& ref);
};

/* These two utility functions are added as syntax-sugar so our eyes don't bleed as much casting
 * pointers to member functions to resolve linker name-based resolution.  Taken 2010/10/16 from
 * http://stackoverflow.com/questions/3946955/unable-to-resolve-overloaded-class-methods-in-template-delegate/3948560#3948560
 * Thanks, UncleBens!
 */
template <class ObjectType, class DataType>
DataType (ObjectType::*as_getter(DataType (ObjectType::*g)(void)const))() const {
  return g;
}

template <class ObjectType, class DataType>
void (ObjectType::*as_setter(void (ObjectType::*s)(const DataType&)))(const DataType&) {
  return s;
}

#endif // #ifndef SYMPHONY_CL_PROPERTY_NODE
