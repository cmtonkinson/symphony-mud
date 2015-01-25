
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

    void                                    create(void);

  public:
    Area(void);
    Area(unsigned long low, unsigned long high);
    ~Area(void);

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
    std::map<unsigned long,Object*>&        objects(void)                   { return _objects; }
    const std::map<unsigned long,Object*>&  objects(void) const             { return _objects; }
    std::map<unsigned long,Mob*>&           mobs(void)                      { return _mobs; }
    const std::map<unsigned long,Mob*>&     mobs(void) const                { return _mobs; }
    void                                    terrain(Terrain* terrain)       { _terrain = terrain; }
    Terrain*                                terrain(void)                   { return _terrain; }

    // General methods...
    bool            load(ROW& row);
    bool            loadRooms(void);
    bool            loadExits(void);
    void            loadObjects(Mysql* db);
    bool            loadMobs(void);
    void            save(Mysql* db);
    Room*           createRoom(void);
    bool            destroy(Mysql* db);
    unsigned long   lowestAvailableRoom(void);
    void            reset(void);
    bool            reset(RecurringJob* job)                 { reset(); return true; }
    unsigned        howManyMobs(unsigned long vnum);
    void            setTerrain(const char* terrain_name);

};

class area_comp { /* Area vnum comparison functor */
  public:
    bool operator () (Area* left, Area* right) {
      return left->low() < right->low();
    }
};

#endif // #ifndef H_SYMPHONY_AREA
