
#ifndef H_SYMPHONY_EXIT
#define H_SYMPHONY_EXIT

#include "enum-table.h"
#include "flag-table.h"

class Room;

class Exit {
  public:
    Exit(void);
    Exit(Room* room, Room* targetRoom_, unsigned direction);
    ~Exit(void);

    // Public accessor methods...
    void                  ID(long ID)                     { _ID = ID; }
    long                  ID(void) const                  { return _ID; }
    void                  targetVnum(unsigned targetVnum) { _targetVnum = targetVnum; }
    unsigned              targetVnum(void) const          { return _targetVnum; }
    void                  targetRoom(Room* targetRoom)    { _targetRoom = targetRoom; }
    Room*                 targetRoom(void);
    Direction&            direction(void)                 { return _direction; }
    const Direction&      direction(void) const           { return _direction; }
    FlagBank&             flags(void)                     { return _flags; }
    const FlagBank&       flags(void) const               { return _flags; }
    void                  key(long key)                   { _key = key; }
    long                  key(void) const                 { return _key; }
    void                  direction(unsigned x)           { _direction.set(x); }

    // General methods...
    void                  flag(const unsigned long& flag, const bool& value, bool stop = false);
    bool                  flag(long flag)         { return _flags.test(flag); }
    void                  destroy(void);

    // Statics...
    static unsigned       inverse(unsigned direction);
    static const char*    name(unsigned direction);
    static const char*    inverseName(unsigned direction);
    static unsigned       string2dir(const std::string& str);
    static unsigned       string2inverse(const std::string& str);
    static const char*    getFlagName(long flag);
    static long           getFlagType(const std::string& flag);

  private:
    long      _ID;
    unsigned  _targetVnum;
    Room*     _targetRoom;
    Direction _direction;
    FlagBank  _flags;
    long      _key;
};

#endif // #ifndef H_SYMPHONY_EXIT
