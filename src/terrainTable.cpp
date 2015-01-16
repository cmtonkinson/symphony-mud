/*
 * $Id: terrainTable.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#include "display.h"
#include "regex.h"
#include "terrainTable.h"

TerrainTable::TerrainTable(void) {
  add(new TerrainBeach());
  add(new TerrainCastle());
  add(new TerrainCave());
  add(new TerrainCity());
  add(new TerrainChurch());
  add(new TerrainDeepwater());
  add(new TerrainDesert());
  add(new TerrainField());
  add(new TerrainForest());
  add(new TerrainGraveyard());
  add(new TerrainIndoor());
  add(new TerrainJungle());
  add(new TerrainMine());
  add(new TerrainMountain());
  add(new TerrainPlains());
  add(new TerrainRoad());
  add(new TerrainSnow());
  add(new TerrainStone());
  add(new TerrainSwamp());
  add(new TerrainTreetop());
  add(new TerrainUnderwater());
  add(new TerrainVillage());
  add(new TerrainVolcano());
  add(new TerrainWater());
  return;
}

TerrainTable::~TerrainTable(void) {
  return;
}

void TerrainTable::add(Terrain* t) {
  terrains().insert(std::make_pair(t->name(), t));
  return;
}

Terrain* TerrainTable::find(const std::string& name) const {
  std::map<std::string,Terrain*>::const_iterator it;
  if ((it = terrains().find(name)) != terrains().end()) {
    return it->second;
  }
  for (it = terrains().begin(); it != terrains().end(); ++it) {
    if (Regex::strPrefix(name, it->second->name())) {
      return it->second;
    }
  }
  return NULL;
}

std::string TerrainTable::list(void) const {
  std::vector<std::string> items;
  std::string name_with_color;
  for (std::map<std::string,Terrain*>::const_iterator it = terrains().begin(); it != terrains().end(); ++it) {
    name_with_color.assign(1, '{').append(1, it->second->map()).append(it->second->name()).append(1, '{').append(1, 'x');
    items.push_back(name_with_color);
  }
  return Display::formatColumns(items);
}
