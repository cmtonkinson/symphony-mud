
#ifndef H_SYMPHONY_ZONE
#define H_SYMPHONY_ZONE

#include <ctime>
#include <map>
#include <set>
#include <string>
#include "recurring-job.h"
#include "terrain.h"

class Avatar;
class Room;
class Item;
class Npc;

class Zone {
  private:
    unsigned long                           _ID;
    unsigned long                           _low;
    unsigned long                           _high;
    std::string                             _name;
    std::map<unsigned long,Room*>           _rooms;
    std::map<unsigned long,Item*>         _items;
    std::map<unsigned long,Npc*>            _npcs;
    Terrain*                                _terrain;
    std::set<std::string>                   _builders;

    void                                    create(void);

  public:
    Zone(void);
    Zone(unsigned long low, unsigned long high);
    ~Zone(void);

    // Public accessor methods...
    void                                    ID(unsigned long ID)            { _ID = ID; }
    unsigned long                           ID(void) const                  { return _ID; }
    void                                    low(unsigned long low)          { _low = low; }
    unsigned long                           low(void) const                 { return _low; }
    void                                    high(unsigned long high)        { _high = high; }
    unsigned long                           high(void) const                { return _high; }
    void                                    name(const char* name);
    std::string                             name(void) const                { return _name; }
    std::map<unsigned long,Room*>&          rooms(void)                     { return _rooms; }
    const std::map<unsigned long,Room*>&    rooms(void) const               { return _rooms; }
    std::map<unsigned long,Item*>&        items(void)                   { return _items; }
    const std::map<unsigned long,Item*>&  items(void) const             { return _items; }
    std::map<unsigned long,Npc*>&           npcs(void)                      { return _npcs; }
    const std::map<unsigned long,Npc*>&     npcs(void) const                { return _npcs; }
    void                                    terrain(Terrain* terrain)       { _terrain = terrain; }
    Terrain*                                terrain(void)                   { return _terrain; }
    std::set<std::string>&                  builders(void)                  { return _builders; }
    const std::set<std::string>&            builders(void) const            { return _builders; }

    void            insert(Room* room);
    void            insert(Item* item);
    void            insert(Npc* npc);

    // General methods...
    unsigned long   lowestAvailableRoom(void);
    void            reset(void);
    bool            reset(RecurringJob* job)                 { reset(); return true; }
    unsigned        howManyNpcs(unsigned long vnum);
    void            setTerrain(const char* terrain_name);

    // Setup & teardown...
    void          initialize(void);
    void          save(void);
    static Zone*  load(std::string filename);
    bool          destroy(void);

    // Builder permissions...
    bool        hasPermission(Avatar* avatar) const;
    void        grantPermission(Avatar* avatar);
    void        revokePermission(Avatar* avatar);
    std::string serializeBuilders(void) const;
    void        unserializeBuilders(const std::string& serialization);

};

class zone_comp { /* Zone vnum comparison functor */
  public:
    bool operator () (Zone* left, Zone* right) {
      return left->low() < right->low();
    }
};

#endif // #ifndef H_SYMPHONY_ZONE
