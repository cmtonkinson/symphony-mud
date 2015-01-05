/*
 * $Id: dice.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "dice.h"

Dice::Dice( void ):
    _number( 0 ),
    _faces( 0 ) {
  return;
}

Dice::Dice( const Dice& ref ):
    _number( ref.number() ),
    _faces( ref.faces() ) {
  return;
}

Dice::Dice( const unsigned short& number, const unsigned short& faces ):
    _number( number ),
    _faces( faces ) {
  return;
}

Dice::~Dice( void ) {
  return;
}

unsigned Dice::roll( void ) const {
  unsigned total = 0;
  for ( int i=0; i < number(); ++i ) {
    total += rand() % faces() + 1;
  }
  return total;
}

unsigned Dice::average( void ) const {
  double foo = ( 1.0 + (double)faces() ) / 2.0; // average for one die
  return foo * (double)number();                // multiply by number of dice
}

const char* Dice::toString( void ) const {
  std::string dest;
  dest.assign( estring( number() ) ).append( 1, 'd' ).append( estring( faces() ) );
  return dest.c_str();
}
