/* Symphony MUD copyright 2005-2015 Chris Tonkinson <chris@tonkinson.com> */

#ifndef H_SYMPHONY_GROUP
#define H_SYMPHONY_GROUP

#include <set>
#include "room.h"

class Creature;

class Group {
  public:
    // constructors...
    Group(void);
    ~Group( void );

    // public accessors...
    std::set<Creature*>&    members(void) { return _members; }
    Creature*               leader(void)  { return _leader; }
    void                    leader(Creature* new_leader);

    // general methods...
    void                    add(Creature* member);
    void                    remove(Creature* member);
    void                    send(std::string format,
                              Creature* creature,
                              void* arg1 = NULL,
                              void* arg2 = NULL,
                              const unsigned long& target = TO_ROOM);

  private:
    // data...
    std::set<Creature*>     _members;
    Creature*               _leader;

};

#endif // !H_SYMPHONY_GROUP
