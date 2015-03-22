
#include "display.h"
#include "regex.h"
#include "terrain-table.h"

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
