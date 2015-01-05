/*
 * $Id: object-container.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#include "object-container.h"

ObjContainer::ObjContainer( void ):
    _inventory( &Identifiers::shortname ) {
  return;
}

ObjContainer::ObjContainer( const ObjContainer& ref ):
    _inventory( ref.inventory() ),
    _flags( ref.flags() ) {
  return;
}

ObjContainer::ObjContainer( ROW row ):
    _inventory( &Identifiers::shortname ) {
  return;
}

ObjContainer::~ObjContainer( void ) {
  return;
}
