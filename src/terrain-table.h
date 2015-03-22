
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
