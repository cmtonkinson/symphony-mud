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
    std::set<Creature*>&    members(void)       { return _members; }
    Creature*               leader(void)        { return _leader; }
    void                    leader(Creature* new_leader);
    std::set<Creature*>&    opponents(void)  { return _opponents; }

    // general methods...
    void                    add_member(Creature* member);
    void                    remove_member(Creature* member);
    void                    add_opponent(Creature* opponent);
    void                    remove_opponent(Creature* opponent);
    void                    send(std::string format,
                              Creature* creature,
                              void* arg1 = NULL,
                              void* arg2 = NULL,
                              const unsigned long& target = TO_ROOM);
    bool                    in_combat(void)     { return !_opponents.empty(); }

  private:
    // data...
    std::set<Creature*>     _members;
    Creature*               _leader;
    std::set<Creature*>     _opponents;

};

#endif // !H_SYMPHONY_GROUP
