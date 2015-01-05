/*
 * $Id: dice.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_DICE
#define H_SYMPHONY_DICE

#include <math.h>
#include <stdlib.h>
#include "estring.h"

class Dice {
  public:
    // constructors
    Dice( void );
    Dice( const Dice& ref );
    Dice( const unsigned short& number, const unsigned short& faces );
    ~Dice( void );

    // public accessor methods
    void            number( const unsigned short& number )  { _number = number; }
    unsigned short  number( void ) const                    { return _number; }
    void            faces( const unsigned short& faces )    { _faces = faces; }
    unsigned short  faces( void ) const                     { return _faces; }

    // general methods
    unsigned    roll( void ) const;
    unsigned    average( void ) const;
    unsigned    min( void ) const                           { return number(); }
    unsigned    max( void ) const                           { return number() * faces(); }
    const char* toString( void ) const;

  private:
    unsigned short  _number;
    unsigned short  _faces;
};

#endif // #ifndef H_SYMPHONY_DICE
