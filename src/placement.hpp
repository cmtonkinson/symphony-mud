
#ifndef H_SYMPHONY_PLACEMENT
#define H_SYMPHONY_PLACEMENT

#include <list>
#include <string>

class Item;
class Npc;
class Room;

class Placement {
  public:

    static const unsigned ERROR  = 0;
    // rule types...
    static const unsigned ITEM = 1;
    static const unsigned NPC    = 2;
    // rule prepositions...
    static const unsigned ON     = 1;  // the indirect item is an item
    static const unsigned IN     = 2;  // the indirect item is an item
    static const unsigned CARRY  = 3;  // the indirect item is a npc
    static const unsigned WEAR   = 4;  // the indirect item is a npc

    // constructors...
    Placement(void);
    virtual ~Placement(void);

    // public accessor methods...
    void            ID(unsigned long ID)                { _ID = ID; }
    unsigned long   ID(void) const                      { return _ID; }
    void            vnum(unsigned long vnum)            { _vnum = vnum; }
    unsigned long   vnum(void) const                    { return _vnum; }
    void            room(Room* room)                    { _room = room; }
    Room*           room(void) const                    { return _room; }
    void            type(unsigned type)                 { _type = type; }
    unsigned        type(void) const                    { return _type; }
    void            target(unsigned long target)        { _target = target; }
    unsigned long   target(void) const                  { return _target; }
    void            number(unsigned number)             { _number = number; }
    unsigned        number(void) const                  { return _number; }
    void            max(unsigned max)                   { _max = max; }
    unsigned        max(void) const                     { return _max; }
    void            probability(unsigned probability)   { _probability = probability; }
    unsigned        probability(void) const             { return _probability; }

    // general methods...
    const char*         strType(void) const             { return (type() == ITEM ? "ITEM" : "NPC"); }
    virtual std::string notes(void) const = 0;
    virtual void        destroy(void) = 0;
    virtual bool        execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs) = 0;

    static Placement*    create(const char* type);

  private:

    unsigned long   _ID;
    unsigned long   _vnum;
    Room*           _room;
    unsigned        _type;            // item or npc
    unsigned long   _target;          // vnum of the item/npc
    unsigned        _number;          // number to load per reset
    unsigned        _max;             // max in the zone for npcs, max in the room for items
    unsigned        _probability;     // 1-100 percent probability that the item/npc will load per reset
};

#endif // #ifndef H_SYMPHONY_PLACEMENT
