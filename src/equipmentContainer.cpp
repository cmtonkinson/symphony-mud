/*
 * $Id: equipmentContainer.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "creature.h"
#include "equipmentContainer.h"
#include "object.h"

EquipmentContainer::EquipmentContainer( void ) {
  return;
}

EquipmentContainer::EquipmentContainer( const EquipmentContainer& ref ) {
  for ( std::map<int,Object*>::const_iterator it = ref.objectMap().begin(); it != ref.objectMap().end(); ++it ) {
    objectMap()[it->first] = new Object( *(it->second) );
  }
  return;
}

EquipmentContainer::~EquipmentContainer( void ) {
  purgeObjects();
  return;
}

void EquipmentContainer::add( Object* object, const int& location ) {
  objectMap()[location] = object;
  return;
}

void EquipmentContainer::remove( Object* object ) {
  for ( std::map<int,Object*>::iterator it = objectMap().begin(); it != objectMap().end(); ++it ) {
    if ( it->second == object ) {
      objectMap().erase( it );
      break;
    }
  }
  return;
}

void EquipmentContainer::remove( const std::list<Object*>& objects ) {
  for ( std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
    remove( *it );
  }
  return;
}

void EquipmentContainer::purgeObjects( void ) {
  for ( std::map<int,Object*>::iterator it = objectMap().begin(); it != objectMap().end(); ++it ) {
    if ( it->second ) {
      delete it->second;
    }
  }
  objectMap().clear();
  return;
}

std::list<Object*> EquipmentContainer::searchObjects( const std::string& q ) {
  std::vector<std::string> keywords;
  int multiplier = 0;
  int index = 0;
  keywords = Container::parseQuery( q, multiplier, index );
  return Container::search( objectMap(), keywords, multiplier, index );
}

Object* EquipmentContainer::searchSingleObject( const std::string& q ) {
  std::list<Object*> l = searchObjects( q );
  if ( l.empty() ) {
    return NULL;
  } else {
    return l.front();
  }
}

std::string EquipmentContainer::listObjects( void ) const {
  std::map<std::string,unsigned> map;
  std::string dest;
  for ( std::map<int,Object*>::const_iterator it = objectMap().begin(); it != objectMap().end(); ++it ) {
    dest.append( Creature::wearLocName( it->first ) );
    dest.append( it->second->decorativeShortname() );
    dest.append( "\n" );
  }
  return Regex::trim(dest);
}
