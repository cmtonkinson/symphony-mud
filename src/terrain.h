
#ifndef H_SYMPHONY_TERRAIN
#define H_SYMPHONY_TERRAIN

#include <string>

class Being;

class Terrain {
  public:

    static const unsigned DEFAULT_MOVEMENT = 5;

    Terrain(void);
    virtual ~Terrain(void);

    // Public accessor methods...
    void          name(std::string name)          { _name = name; }
    std::string   name(void) const                { return _name; }
    void          base(unsigned base)             { _base = base; }
    unsigned      base(void) const                { return _base; }
    void          title(char title)               { _title = title; }
    char          title(void) const               { return _title; }
    void          description(char description)   { _description = description; }
    char          description(void) const         { return _description; }
    void          map(const char map)             { _map = map; }
    char          map(void) const                 { return _map; }
    void          flags(unsigned flags)           { _flags = flags; }
    unsigned      flags(void) const               { return _flags; }
    void          flag(unsigned flag, bool value) { flags(value ? (flags()|flag) : (flags()&(~flag))); }
    bool          flag(unsigned flag)             { return ((flags()&flag) == flag ? true : false); }

    // Public methods...
    virtual unsigned  calculate(Being* being) = 0;

  private:
    std::string _name;
    unsigned    _base;
    char        _title;
    char        _description;
    char        _map;
    unsigned    _flags;
};

#define DEF_TERRAIN(NAME)                             \
class NAME: public Terrain {                          \
  public:                                             \
    NAME(void);                                       \
    virtual ~NAME(void) { }                           \
    virtual unsigned  calculate(Being* being);  \
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
