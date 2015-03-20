
#ifndef H_SYMPHONY_FLAG_TABLE
#define H_SYMPHONY_FLAG_TABLE

#include <map>
#include <string>
#include "flagBank.h"

typedef std::map<std::string,FlagInt> NameToVal;
typedef std::map<FlagInt,std::string> ValToName;

class FlagTable {
  public:
    // constructors...
    FlagTable(void);
    virtual ~FlagTable(void);

    // general methods...
    void        add(const std::string& name, const FlagInt& value, std::string decorative = "");
    FlagInt     find(const std::string& name);    // looks for an exact match
    FlagInt     search(const std::string& name);  // looks for a prefix match
    const char* find(const FlagInt& value);
    std::string dump(void) const;

    // accessors...
    NameToVal&        nameToValue(void)           { return _nameToValue; }
    const NameToVal&  nameToValue(void) const     { return _nameToValue; }
    ValToName&        valueToName(void)           { return _valueToName; }
    const ValToName&  valueToName(void) const     { return _valueToName; }
    ValToName&        decorativeName(void)        { return _decorativeName; }
    const ValToName&  decorativeName(void) const  { return _decorativeName; }

  private:
    // data...
    NameToVal _nameToValue;
    ValToName _valueToName;
    ValToName _decorativeName;
};

#define DEF_FLAGTABLE(NAME)           \
class NAME: public FlagTable {        \
  public:                             \
    NAME(void);                     \
    virtual ~NAME(void) { }         \
    static NAME& Instance(void) {   \
      static NAME _instance;          \
      return _instance;               \
    }                                 \
};

//////////////////////////////////////////////// Avatar...
#define ADMIN_HEADBUILDER      1
#define ADMIN_INCOGNITO        2
#define ADMIN_BIGBROTHER       3
#define ADMIN_BIGBRO_DEATHS    4
#define ADMIN_BIGBRO_ERRORS    5
#define ADMIN_BIGBRO_LOGINS    6
#define ADMIN_BIGBRO_MODES     7
#define ADMIN_BIGBRO_RESETS    8
#define ADMIN_BIGBRO_SYSTEM    9
#define ADMIN_BIGBRO_EVENTS   10
#define ADMIN_BIGBRO_CHANGES  11
DEF_FLAGTABLE(FTAvatarAdmin)

#define CHANNEL_NEWBIE 1
#define CHANNEL_OOC    2
#define CHANNEL_TELL   3
DEF_FLAGTABLE(FTAvatarChannel)

#define WHO_AFK    1
#define WHO_BUSY   2
#define WHO_BORED  3
#define WHO_DUNCE  4
#define WHO_LFG    5
DEF_FLAGTABLE(FTAvatarWho)

//////////////////////////////////////////////// Command...
#define COM_ENABLED     1
#define COM_PLAYERONLY  2
#define COM_SPECIAL     3
#define COM_SOCIAL      4
#define COM_AUDIBLE     5
#define COM_TACTILE     6
DEF_FLAGTABLE(FTCommand)

//////////////////////////////////////////////// Item...
#define ITEM_NOREMOVE   1
#define ITEM_NODROP     2
#define ITEM_NOGET      3
#define ITEM_MELTDROP   4
#define ITEM_GLOWING    5
#define ITEM_HUMMING    6
#define ITEM_BURNPROOF  7
DEF_FLAGTABLE(FTItem)

#define CONTAINER_CLOSEABLE 1
#define CONTAINER_CLOSED    2
#define CONTAINER_LOCKABLE  3
#define CONTAINER_LOCKED    4
DEF_FLAGTABLE(FTContianer)

#define WEAPON_BURNING  1
#define WEAPON_FREEZING 2
#define WEAPON_SHOCKING 3
#define WEAPON_SHARP    4
DEF_FLAGTABLE(FTWeapon)

//////////////////////////////////////////////// Exit...
#define EXIT_CLOSEABLE  1
#define EXIT_CLOSED     2
#define EXIT_LOCKABLE   3
#define EXIT_LOCKED     4
#define EXIT_HIDDEN     5
DEF_FLAGTABLE(FTExit)

//////////////////////////////////////////////// Compound...
#define COMPOUND_METAL  1
#define COMPOUND_STONE  2
#define COMPOUND_GEM    3
#define COMPOUND_WOOD   4
#define COMPOUND_SKIN   5
#define COMPOUND_CLOTH  6
DEF_FLAGTABLE(FTCompound)

//////////////////////////////////////////////// Room...
#define ROOM_NOMAP      1
#define ROOM_NOGATE     2
DEF_FLAGTABLE(FTRoom)

#endif // H_SYMPHONY_FLAG_TABLE
