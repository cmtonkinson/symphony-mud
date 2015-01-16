/*
 * $Id: terrainTable.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_TERRAIN_TABLE
#define H_SYMPHONY_TERRAIN_TABLE

#include <map>
#include "terrain.h"

class TerrainTable {
  public:
    TerrainTable(void);
    ~TerrainTable(void);

    // Public accessor methods...
    std::map<std::string,Terrain*>&         terrains(void)               { return _terrains; }
    const std::map<std::string,Terrain*>&   terrains(void) const         { return _terrains; }

    // General methods...
    void        add(Terrain*);
    Terrain*    find(const std::string& name) const;
    std::string list(void) const;

    static TerrainTable&  Instance(void);

  private:
    std::map<std::string,Terrain*>    _terrains;
};

inline TerrainTable& TerrainTable::Instance(void) { // public static
  static TerrainTable instance;
  return instance;
}

#endif // #ifndef H_SYMPHONY_TERRAIN_TABLE
