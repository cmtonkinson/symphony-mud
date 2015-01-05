/*
 * $Id: object-weapon.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#include "object-weapon.h"
#include "regex.h"

ObjWeapon::ObjWeapon( void ) {
  type().set( 0 );
  verb().set( 0 );
  return;
}

ObjWeapon::ObjWeapon( const ObjWeapon& ref ):
    _type( ref.type() ),
    _verb( ref.verb() ),
    _flags( ref.flags() ),
    _damage( ref.damage() ) {
  return;
}

ObjWeapon::ObjWeapon( ROW row ) {
  type().set( (int)row["weapon_type"] );
  verb().set( (int)row["weapon_verb"] );
  //flags( row["weapon_flags"] );
  damage().number( row["weapon_damage_number"] );
  damage().faces( row["weapon_damage_faces"] );
  return;
}

ObjWeapon::~ObjWeapon( void ) {
  return;
}
