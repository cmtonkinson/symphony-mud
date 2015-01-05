/*
 * $Id: terrain.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_TERRAIN
#define H_SYMPHONY_TERRAIN

#include <string>

#define DEFAULT_MOVEMENT  10

class Creature;

class Terrain {
  public:
    Terrain( void );
    virtual ~Terrain( void );

    // Public accessor methods...
    void                    name( const std::string& name )                       { _name = name; }
    std::string             name( void ) const                                    { return _name; }
    void                    base( const unsigned short& base )                    { _base = base; }
    unsigned short          base( void ) const                                    { return _base; }
    void                    title( const char title )                             { _title = title; }
    char                    title( void ) const                                   { return _title; }
    void                    description( const char description )                 { _description = description; }
    char                    description( void ) const                             { return _description; }
    void                    map( const char map )                                 { _map = map; }
    char                    map( void ) const                                     { return _map; }
    void                    flags( const unsigned long& flags )                   { _flags = flags; }
    unsigned long           flags( void ) const                                   { return _flags; }
    void                    flag( const unsigned long& flag, const bool& value )  { flags( value ? (flags()|flag) : (flags()&(~flag)) ); }
    bool                    flag( const unsigned long& flag )                     { return ( (flags()&flag) == flag ? true : false ); }

    // Public methods...
    virtual unsigned short  calculate( Creature* creature )                       = 0;

  private:
    std::string             _name;
    unsigned short          _base;
    char                    _title;
    char                    _description;
    char                    _map;
    unsigned long           _flags;
};

#define DEF_TERRAIN(NAME)                                     \
class NAME: public Terrain {                                  \
  public:                                                     \
    NAME( void );                                             \
    virtual ~NAME( void ) { }                                 \
    virtual unsigned short  calculate( Creature* creature );  \
};

DEF_TERRAIN(TerrainBeach)
DEF_TERRAIN(TerrainCastle)
DEF_TERRAIN(TerrainCave)
DEF_TERRAIN(TerrainCity)
DEF_TERRAIN(TerrainChurch)
DEF_TERRAIN(TerrainDeepwater)
DEF_TERRAIN(TerrainDesert)
DEF_TERRAIN(TerrainField)
DEF_TERRAIN(TerrainForest)
DEF_TERRAIN(TerrainGraveyard)
DEF_TERRAIN(TerrainIndoor)
DEF_TERRAIN(TerrainJungle)
DEF_TERRAIN(TerrainMine)
DEF_TERRAIN(TerrainMountain)
DEF_TERRAIN(TerrainPlains)
DEF_TERRAIN(TerrainRoad)
DEF_TERRAIN(TerrainSnow)
DEF_TERRAIN(TerrainStone)
DEF_TERRAIN(TerrainSwamp)
DEF_TERRAIN(TerrainTreetop)
DEF_TERRAIN(TerrainUnderwater)
DEF_TERRAIN(TerrainVillage)
DEF_TERRAIN(TerrainVolcano)
DEF_TERRAIN(TerrainWater)

#endif // #ifndef H_SYMPHONY_TERRAIN
