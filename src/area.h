/*
 * $Id: area.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_AREA
#define H_SYMPHONY_AREA

#include <ctime>
#include <map>
#include <string>
#include "mysql.h"
#include "recurring-job.h"
#include "terrain.h"

class Room;
class Object;
class Mob;

class Area {
  private:
    unsigned long                           _ID;
    unsigned long                           _low;
    unsigned long                           _high;
    std::string                             _name;
    std::map<unsigned long,Room*>           _rooms;
    std::map<unsigned long,Object*>         _objects;
    std::map<unsigned long,Mob*>            _mobs;
    Terrain*                                _terrain;

    void                                    create( void );

  public:
    Area( void );
    Area( const unsigned long& low, const unsigned long& high );
    ~Area( void );

    // Public accessor methods...
    void                                    ID( const unsigned long& ID )             { _ID = ID; }
    unsigned long                           ID( void ) const                          { return _ID; }
    void                                    low( const unsigned long& low )           { _low = low; }
    unsigned long                           low( void ) const                         { return _low; }
    void                                    high( const unsigned long& high )         { _high = high; }
    unsigned long                           high( void ) const                        { return _high; }
    void                                    name( const std::string& name );
    std::string                             name( void ) const                        { return _name; }
    std::map<unsigned long,Room*>&          rooms( void )                             { return _rooms; }
    const std::map<unsigned long,Room*>&    rooms( void ) const                       { return _rooms; }
    std::map<unsigned long,Object*>&        objects( void )                           { return _objects; }
    const std::map<unsigned long,Object*>&  objects( void ) const                     { return _objects; }
    std::map<unsigned long,Mob*>&           mobs( void )                              { return _mobs; }
    const std::map<unsigned long,Mob*>&     mobs( void ) const                        { return _mobs; }
    void                                    terrain( Terrain* terrain )               { _terrain = terrain; }
    Terrain*                                terrain( void )                           { return _terrain; }

    // General methods...
    bool                                    load( ROW& row );
    bool                                    loadRooms( void );
    bool                                    loadExits( void );
    void                                    loadObjects( Mysql* db );
    bool                                    loadMobs( void );
    void                                    save( Mysql* db );
    Room*                                   createRoom( void );
    bool                                    destroy( Mysql* db );
    unsigned long                           lowestAvailableRoom( void );
    void                                    reset( void );
    bool                                    reset( RecurringJob* job )                 { reset(); return true; }
    unsigned                                howManyMobs( const unsigned long& vnum );

};

class area_comp { /* Area vnum comparison functor */
  public:
    bool operator () ( Area* left, Area* right ) {
      return left->low() < right->low();
    }
};

#endif // #ifndef H_SYMPHONY_AREA
