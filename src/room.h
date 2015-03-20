
#ifndef H_SYMPHONY_ROOM
#define H_SYMPHONY_ROOM

#include <list>
#include <string>
#include "inventoryContainer.h"
#include "item.h"
#include "terrain.h"


class Zone;
class Being;
class Exit;
class Placement;

class Room {
  public:

    static const unsigned INDEX_DEFAULT = (unsigned)-1;

    // Message targets...
    static const unsigned TO_BEING = 0;
    static const unsigned TO_VICT     = 1;
    static const unsigned TO_NOTVICT  = 2;
    static const unsigned TO_ROOM     = 3;
    static const unsigned TO_ALL      = 4;

    Room(void);
    Room(unsigned long vnum, Zone* zone);
    ~Room(void);

    // Public accessor methods...
    void                          ID(unsigned long ID)                    { _ID = ID; }
    unsigned long                 ID(void) const                          { return _ID; }
    void                          zone(Zone* zone)                        { _zone = zone; }
    Zone*                         zone(void)                              { return _zone; }
    void                          vnum(unsigned long vnum)                { _vnum = vnum; }
    unsigned long                 vnum(void) const                        { return _vnum; }
    void                          name(std::string name)                  { _name = name; }
    std::string                   name(void) const                        { return _name; }
    void                          description(std::string description)    { _description = description; }
    std::string                   description(void) const                 { return _description; }
    std::string*                  descriptionp(void)                      { return &_description; } // have to expose this for the text editor
    void                          smell(std::string smell)                { _smell = smell; }
    std::string                   smell(void) const                       { return _smell; }
    void                          sound(std::string sound)                { _sound = sound; }
    std::string                   sound(void) const                       { return _sound; }
    void                          terrain(Terrain* terrain)               { _terrain = terrain; }
    Terrain*                      terrain(void)                           { return _terrain; }
    void                          flags(const FlagBank& flags)            { _flags = flags; }
    FlagBank&                     flags(void)                             { return _flags; }
    const FlagBank&               flags(void) const                       { return _flags; }
    void                          exit(unsigned direction, Exit* exit);
    Exit*                         exit(unsigned direction);
    Exit*                         exit(std::string direction);
    std::list<Being*>&         beings(void)                         { return _beings; }
    const std::list<Being*>&   beings(void) const                   { return _beings; }
    InventoryContainer&           inventory(void)                         { return _inventory; }
    const InventoryContainer&     inventory(void) const                   { return _inventory; }
    std::list<Placement*>&         placements(void)                         { return _placements; }
    const std::list<Placement*>&   placements(void) const                   { return _placements; }

    // General methods...
    bool                          loadExits(void);
    void                          add(Being* being)                                                       { beings().push_back(being); }
    void                          remove(Being* being)                                                    { beings().remove(being); }
    void                          add(Placement* placement)                                                       { placements().push_back(placement); }
    void                          removePlacement(unsigned index);
    void                          executePlacements(void);
    void                          add(Item* item)                                                           { inventory().add(item); }
    void                          remove(Item* item)                                                        { inventory().remove(item); }
    void                          send(std::string format, Being* being = NULL, void* arg1 = NULL,
                                        void* arg2 = NULL, unsigned long target = TO_ROOM);
    void                          send_cond(std::string format, Being* being = NULL, void* arg1 = NULL,
                                        void* arg2 = NULL, unsigned long target = TO_ROOM, bool audible = false);
    void                          destroy(void);
    bool                          clear(void);
    std::string                   listFlags(void);
    void                          reset(void);
    Being*                     being_by_vnum(unsigned long vnum, unsigned index = INDEX_DEFAULT);
    void                          setTerrain(const char* terrain_name);

    // Statics...
    static const char*    getFlagName(unsigned long flag);
    static unsigned long  getFlagType(std::string flag);
    static std::string    getInformation(Room* room);

  private:
    unsigned long           _ID;
    Zone*                   _zone;
    unsigned long           _vnum;
    std::string             _name;
    std::string             _description;
    std::string             _smell;
    std::string             _sound;
    Terrain*                _terrain;
    FlagBank                _flags;
    Exit*                   _exits[6];
    std::list<Being*>    _beings;
    InventoryContainer      _inventory;
    std::list<Placement*>    _placements;
};

#endif // #ifndef H_SYMPHONY_ROOM
