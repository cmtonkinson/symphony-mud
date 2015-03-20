
#ifndef SYMPHONY_ENUMTABLE_H
#define SYMPHONY_ENUMTABLE_H

#include <map>
#include <string>
#include "enum.h"
#include "flagBank.h"

class EnumTable {
  public:
    // constructors...
    EnumTable(void);
    virtual ~EnumTable(void);

    // general methods...
    void        add(const EnumInt& n, const std::string& s);
    EnumInt     get(const std::string& value) const;
    std::string get(const EnumInt& value) const;
    std::string list(void) const;

  private:
    std::map<EnumInt,std::string> _int2string;
    std::map<std::string,EnumInt> _string2int;
};

#define DEF_ENUM(NAME)                      \
class ET##NAME: public EnumTable {          \
  public:                                   \
    ET##NAME(void);                         \
    virtual ~ET##NAME(void) { }             \
    static ET##NAME& Instance(void) {       \
      static ET##NAME _instance;            \
      return _instance;                     \
    }                                       \
};                                          \
class NAME: public Enum {                   \
  public:                                   \
    NAME(EnumInt value = 0) {               \
      Enum::_table = &ET##NAME::Instance(); \
      set(value);                           \
    }                                       \
    virtual ~NAME(void) { }                 \
};                                          \

#define UNDEFINED   0

DEF_ENUM(Mode)
#define MODE_NONE   1
#define MODE_ZEDIT  2
#define MODE_MEDIT  3
#define MODE_IEDIT  4
#define MODE_PEDIT  5
#define MODE_REDIT  6
#define MODE_SEDIT  7

DEF_ENUM(Status)
#define CONNECTING  1
#define CREATING    2
#define DELETING    3
#define CONNECTED   4

DEF_ENUM(Composing)
#define COMP_NOTE   1

DEF_ENUM(Direction)
#define NORTH   1
#define EAST    2
#define SOUTH   3
#define WEST    4
#define UP      5
#define DOWN    6

DEF_ENUM(FurniturePosition)
#define FURN_LAY_ON   1
#define FURN_SIT_AT   2
#define FURN_SIT_ON   3
#define FURN_STAND_ON 4

/*DEF_ENUM(ItemType)
#define OBJ_ARMOR     1
#define OBJ_CLOTHING  2
#define OBJ_CONTAINER 3
#define OBJ_FOOD      4
#define OBJ_FURNITURE 5
#define OBJ_JEWELRY   6
#define OBJ_KEY       7
#define OBJ_TRASH     8
#define OBJ_WEAPON    9*/

DEF_ENUM(WeaponType)
#define WEAP_AXE         1
#define WEAP_CLUB        2
#define WEAP_DAGGER      3
#define WEAP_FLAIL       4
#define WEAP_GAUNTLET    5
#define WEAP_LONGSWORD   6
#define WEAP_MACE        7
#define WEAP_POLEARM     8
#define WEAP_SHORTSWORD  9
#define WEAP_STAFF      10
#define WEAP_WHIP       11

DEF_ENUM(DamageVerb)
// bash
#define DAM_BASH      100
#define DAM_BEATING   101
#define DAM_CRUSH     102
#define DAM_POUND     103
#define DAM_PUNCH     104
#define DAM_SLAM      105
#define DAM_SMACK     106
#define DAM_SMASH     107
#define DAM_STRIKE    108
#define DAM_THWACK    109
// slash
#define DAM_CHOP      200
#define DAM_CLEAVE    201
#define DAM_CUT       202
#define DAM_HACK      203
#define DAM_SLASH     204
#define DAM_SLICE     205
// pierce
#define DAM_BITE      300
#define DAM_PIERCE    301
#define DAM_POKE      302
#define DAM_PRICK     303
#define DAM_PUNCTURE  304
#define DAM_STAB      305
#define DAM_STING     306
#define DAM_THRUST    307
// exotic
#define DAM_WRATH     400

DEF_ENUM(Slot)
#define HEAD       1
#define EAR        2 //x2
#define NECK       3
#define SHOULDERS  4
#define ARMS       5
#define TORSO      6
#define HANDS      7
#define FINGER     8 //x2
#define HOLD       9 //x2
#define WAIST     10
#define LEGS      11
#define FEET      12

DEF_ENUM(Position) // must be in powers of 2
#define LAYING    A
#define SITTING   B
#define STANDING  C
#define MOUNTED   D

DEF_ENUM(Action) // must be in powers of 2
#define DEAD      A
#define SLEEPING  B
#define FIGHTING  C

DEF_ENUM(Gender)
#define MALE    1
#define FEMALE  2
#define NEUTRAL 3

DEF_ENUM(Race)
#define DWARF   1
#define ELF     2
#define HUMAN   3

DEF_ENUM(PClass)
#define CLERIC    1
#define MAGE      2
#define ROGUE     3
#define WARRIOR   4

#endif // SYMPHONY_ENUMTABLE_H
