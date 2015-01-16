
#ifndef H_SYMPHONY_LOADRULE
#define H_SYMPHONY_LOADRULE

#include <list>
#include "mysql.h"

class Mob;
class Object;
class Room;

#define LOADRULE_ERROR    0

// rule types...
#define LOADRULE_OBJECT   1
#define LOADRULE_MOB      2

// rule prepositions...
#define LOADRULE_ON       1   // the indirect object is an object
#define LOADRULE_IN       2   // the indirect object is an object
#define LOADRULE_CARRY    3   // the indirect object is a mob
#define LOADRULE_WEAR     4   // the indirect object is a mob



class LoadRule {
  private:
    unsigned long       _ID;
    unsigned long       _vnum;
    Room*               _room;
    unsigned short      _type;
    unsigned long       _target;          // vnum
    unsigned short      _number;          // number to load per reset
    unsigned short      _max;             // max number allowed in area
    unsigned short      _probability;     // 1-100

  public:
    // constructors...
    LoadRule(void);
    LoadRule(ROW row);
    virtual ~LoadRule(void);

    // public accessor methods...
    void                ID(const unsigned long& ID)                         { _ID = ID; }
    unsigned long       ID(void) const                                      { return _ID; }
    void                vnum(const unsigned long& vnum)                     { _vnum = vnum; }
    unsigned long       vnum(void) const                                    { return _vnum; }
    void                room(Room* room)                                    { _room = room; }
    Room*               room(void) const                                    { return _room; }
    void                type(const unsigned short& type)                    { _type = type; }
    unsigned short      type(void) const                                    { return _type; }
    void                target(const unsigned long& target)                 { _target = target; }
    unsigned long       target(void) const                                  { return _target; }
    void                number(const unsigned short& number)                { _number = number; }
    unsigned short      number(void) const                                  { return _number; }
    void                max(const unsigned short& max)                      { _max = max; }
    unsigned short      max(void) const                                     { return _max; }
    void                probability(const unsigned short& probability)      { _probability = probability; }
    unsigned short      probability(void) const                             { return _probability; }

    // general methods...
    std::string         strType(void) const                                 { return (type() == LOADRULE_OBJECT ? "OBJ" : "MOB"); }
    virtual std::string notes(void) const = 0;
    virtual bool        save(void) = 0;
    virtual bool        commit(void) = 0;
    virtual bool        destroy(void);
    virtual bool        execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs) = 0;

};



#endif // #ifndef H_SYMPHONY_LOADRULE
