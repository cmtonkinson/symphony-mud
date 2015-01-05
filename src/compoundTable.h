/*
 * $Id: compoundTable.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_COMPOUND_TABLE
#define H_SYMPHONY_COMPOUND_TABLE

#include <map>
#include <string>
#include "compound.h"

class CompoundTable {
  private:
    std::map<std::string,Compound*>    _compounds;

  public:
    CompoundTable( void );
    ~CompoundTable( void );

    // Public accessor methods...
    std::map<std::string,Compound*>&         compounds( void )               { return _compounds; }
    const std::map<std::string,Compound*>&   compounds( void ) const         { return _compounds; }

    // General methods...
    void        add( Compound* );
    Compound*   find( const std::string& name ) const;
    std::string list( std::string glue = " " ) const;

    static CompoundTable&  Instance( void );

};

inline CompoundTable& CompoundTable::Instance( void ) { // public static
  static CompoundTable instance;
  return instance;
}

#endif // #ifndef H_SYMPHONY_COMPOUND_TABLE
