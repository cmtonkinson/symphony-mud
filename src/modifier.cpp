/*
 * $Id: modifier.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "modifier.h"

Modifier::Modifier( void ):
    _attribute( 0 ),
    _magnitude( 0 ) {
  return;
}

Modifier::Modifier( const Modifier& ref ):
    _attribute( ref.attribute() ),
    _magnitude( ref.magnitude() ) {
  return;
}

Modifier::Modifier( const unsigned short& attribute, const int& magnitude ):
    _attribute( attribute ),
    _magnitude( magnitude ) {
  return;
}

Modifier::~Modifier( void ) {
  return;
}
