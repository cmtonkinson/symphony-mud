/*
 * $Id: mob.h 369 2010-05-31 15:48:57Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MOB
#define H_SYMPHONY_MOB

#include "creature.h"
#include "mysql.h"

class Area;

class Mob: public Creature {
  public:
    // constructors...
    Mob( void );
    Mob( Area* area, const unsigned long& vnum );
    Mob( const Mob& ref );
    Mob( ROW row );
    virtual ~Mob( void );

    // public accessors...
    void                vnum( const unsigned long& vnum )               { _vnum = vnum; }
    unsigned long       vnum( void ) const                              { return _vnum; }

    // general methods...
    virtual bool        isMob( void ) const                             { return true; }

    // Combat...
    virtual void        die(Creature* killer = NULL);

    // pure virtual overrides...
    virtual bool        save( void );
    virtual bool        destroy( void );

    // statics...
    static Mob*         create( Area* area, const unsigned long& vnum );
    static Mob*         create( ROW row );
    static Mob*         create( Mob* mob, Room* room );
    static std::string  getInformation( Mob* mob );

  private:
    // data...
    unsigned long       _vnum;
};

#endif // !H_SYMPHONY_MOB
