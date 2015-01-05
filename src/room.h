/*
 * $Id: room.h 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_ROOM
#define H_SYMPHONY_ROOM

#include <list>
#include <string>
#include "inventoryContainer.h"
#include "object.h"

#include "terrain.h"

// Message targets...
#define TO_CREATURE 0
#define TO_VICT     1
#define TO_NOTVICT  2
#define TO_ROOM     3
#define TO_ALL      4

class Area;
class Creature;
class Exit;
class LoadRule;

class Room {
  public:
    Room( const unsigned long& vnum, Area* area );
    Room( Area* area, ROW& row );
    ~Room( void );

    // Public accessor methods...
    void                          ID( const unsigned long& ID )                                     { _ID = ID; }
    unsigned long                 ID( void ) const                                                  { return _ID; }
    void                          area( Area* area )                                                { _area = area; }
    Area*                         area( void )                                                      { return _area; }
    void                          vnum( const unsigned long& vnum )                                 { _vnum = vnum; }
    unsigned long                 vnum( void ) const                                                { return _vnum; }
    void                          name( const std::string& name )                                   { _name = name; }
    std::string                   name( void ) const                                                { return _name; }
    void                          description( const std::string& description )                     { _description = description; }
    std::string                   description( void ) const                                         { return _description; }
    std::string*                  descriptionp( void )                                              { return &_description; } // have to expose this for the text editor
    void                          smell( const std::string& smell )                                 { _smell = smell; }
    std::string                   smell( void ) const                                               { return _smell; }
    void                          sound( const std::string& sound )                                 { _sound = sound; }
    std::string                   sound( void ) const                                               { return _sound; }
    void                          terrain( Terrain* terrain )                                       { _terrain = terrain; }
    Terrain*                      terrain( void )                                                   { return _terrain; }
    void                          flags( const FlagBank& flags )                                    { _flags = flags; }
    FlagBank&                     flags( void )                                                     { return _flags; }
    const FlagBank&               flags( void ) const                                               { return _flags; }
    void                          exit( const unsigned short& direction, Exit* exit );
    Exit*                         exit( const unsigned short& direction );
    Exit*                         exit( const std::string& direction );
    std::list<Creature*>&         creatures( void )                                                 { return _creatures; }
    const std::list<Creature*>&   creatures( void ) const                                           { return _creatures; }
    InventoryContainer&           inventory( void )                                                 { return _inventory; }
    const InventoryContainer&     inventory( void ) const                                           { return _inventory; }
    std::list<LoadRule*>&         loadRules( void )                                                 { return _loadRules; }
    const std::list<LoadRule*>&   loadRules( void ) const                                           { return _loadRules; }

    // General methods...
    bool                          loadExits( void );
    void                          add( Creature* creature )                                                       { creatures().push_back( creature ); }
    void                          remove( Creature* creature )                                                    { creatures().remove( creature ); }
    void                          add( LoadRule* loadRule )                                                       { loadRules().push_back( loadRule ); }
    void                          removeLoadRule( const unsigned short& index );
    void                          executeLoadRules( void );
    void                          add( Object* object )                                                           { inventory().add( object ); }
    void                          remove( Object* object )                                                        { inventory().remove( object ); }
    void                          send( std::string format, Creature* creature = NULL, void* arg1 = NULL,
                                        void* arg2 = NULL, const unsigned long& target = TO_ROOM );
    void                          send_cond( std::string format, Creature* creature = NULL, void* arg1 = NULL,
                                        void* arg2 = NULL, const unsigned long& target = TO_ROOM, bool audible = false );
    void                          save( void );
    bool                          destroy( void );
    bool                          clear( void );
    std::string                   listFlags( void );
    void                          reset( void );

    // Statics...
    static const char*    getFlagName( const unsigned long& flag );
    static unsigned long  getFlagType( const std::string& flag );
    static std::string    getInformation( Room* room );

  private:
    unsigned long           _ID;
    Area*                   _area;
    unsigned long           _vnum;
    std::string             _name;
    std::string             _description;
    std::string             _smell;
    std::string             _sound;
    Terrain*                _terrain;
    FlagBank                _flags;
    Exit*                   _exits[6];
    std::list<Creature*>    _creatures;
    InventoryContainer      _inventory;
    std::list<LoadRule*>    _loadRules;
};

#endif // #ifndef H_SYMPHONY_ROOM
