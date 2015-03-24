
#ifndef H_SYMPHONY_GROUP
#define H_SYMPHONY_GROUP

#include <set>
#include "room.h"

class Being;

class Group {
  public:
    // constructors...
    Group(void);
    ~Group(void);

    // public accessors...
    std::set<Being*>&    members(void)       { return _members; }
    Being*               leader(void)        { return _leader; }
    void                    leader(Being* new_leader);

    // general methods...
    size_t  size(void) const                    { return _members.size(); }
    void    add_member(Being* member);
    void    remove_member(Being* member);
    bool    is_member(Being* b);

    void    send(std::string format, Being* being, void* arg1 = NULL, void* arg2 = NULL, const unsigned long& target = Room::TO_ROOM);

  private:
    // data...
    std::set<Being*>     _members;
    Being*               _leader;

};

#endif // !H_SYMPHONY_GROUP
