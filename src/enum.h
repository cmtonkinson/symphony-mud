/*
 * $Id: enum.h 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_ENUM_H
#define SYMPHONY_ENUM_H

#include <string>

typedef unsigned EnumInt;

class EnumTable;

class Enum {
  public:
    // constructors...
    Enum( EnumInt value = 0 );
    Enum( const Enum& ref )                 { *this = ref; }
    virtual ~Enum( void );

    // operators...
    void  operator  = ( const Enum& ref );
    bool  operator == ( const Enum& ref )   { return this->number() == ref.number(); }
    bool  operator  < ( const Enum& ref )   { return this->number()  < ref.number(); }
    bool  operator  > ( const Enum& ref )   { return this->number()  > ref.number(); }

    // accessors...
    EnumInt     number( void ) const;
    std::string string( void ) const;
    void        set( const EnumInt& value );
    void        set( const std::string& value );

    // general methods...
    std::string list( void ) const;

  protected:
    EnumTable*  _table;

  private:
    EnumInt     _value;
};

#endif // SYMPHONY_ENUM_H
