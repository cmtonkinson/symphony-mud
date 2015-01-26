
#ifndef H_SYMPHONY_OBJECT
#define H_SYMPHONY_OBJECT

#include <list>
#include <set>
#include "compound.h"
#include "flagBank.h"
#include "flagTable.h"
#include "identifiers.h"
#include "modifier.h"
#include "mysql.h"

class ObjArmor;
class ObjClothing;
class ObjContainer;
class ObjFood;
class ObjFurniture;
class ObjJewelry;
class ObjKey;
class ObjTrash;
class ObjWeapon;

class Object {
  public:
    // constructors...
    Object(void);
    Object(const Object& ref);
    Object(ROW row);
    ~Object(void);

    // constants...
    enum Type { Type_Undefined = 0, Type_Armor, Type_Clothing, Type_Container, Type_Food, Type_Furniture, Type_Jewelry, Type_Key, Type_Trash, Type_Weapon };
    enum Wearable { Wearable_Undefined = 0, Wearable_Head, Wearable_Ear, Wearable_Face, Wearable_Neck, Wearable_Shoulders, Wearable_Arms, Wearable_Torso, Wearable_Forearm, Wearable_Wrist, Wearable_Hands, Wearable_Hold, Wearable_Finger, Wearable_Waist, Wearable_Legs, Wearable_Knee, Wearable_Shin, Wearable_Ankle, Wearable_Feet };

    // general methods
    bool          isArmor(void) const     { return _type == Type_Armor; }
    bool          isClothing(void) const  { return _type == Type_Clothing; }
    bool          isContainer(void) const { return _type == Type_Container; }
    bool          isFood(void) const      { return _type == Type_Food; }
    bool          isFurniture(void) const { return _type == Type_Furniture; }
    bool          isJewelry(void) const   { return _type == Type_Jewelry; }
    bool          isKey(void) const       { return _type == Type_Key; }
    bool          isTrash(void) const     { return _type == Type_Trash; }
    bool          isWeapon(void) const    { return _type == Type_Weapon; }

    ObjArmor*     armor(void) const       { return (ObjArmor*)_extra; }
    ObjClothing*  clothing(void) const    { return (ObjClothing*)_extra; }
    ObjContainer* container(void) const   { return (ObjContainer*)_extra; }
    ObjFood*      food(void) const        { return (ObjFood*)_extra; }
    ObjFurniture* furniture(void) const   { return (ObjFurniture*)_extra; }
    ObjJewelry*   jewelry(void) const     { return (ObjJewelry*)_extra; }
    ObjKey*       key(void) const         { return (ObjKey*)_extra; }
    ObjTrash*     trash(void) const       { return (ObjTrash*)_extra; }
    ObjWeapon*    weapon(void) const      { return (ObjWeapon*)_extra; }

    const char*   typeToString(void) const;
    void          stringToType(const std::string& src);
    const char*   wearableToString(void) const;
    void          stringToWearable(const std::string& src);

    std::string   serializeModifiers(void) const;
    void          unserializeModifiers(std::string ser);
    std::string   serializeComposition(std::string sep = "~") const;
    void          unserializeComposition(std::string ser);
    std::string   decorativeShortname(void) const;
    std::string   decorativeLongname(void) const;
    std::string   listDecorativeFlags(void) const;
    std::string   printStatus(void) const;

    void          insert(Mysql* db, unsigned long areaID);
    void          update(Mysql* db) const;
    void          destroy(Mysql* db) const;
    void          saveInstance(Mysql* db, unsigned long owner_id, std::string placement, unsigned location) const;
    void          saveContainedInstance(Mysql* db, unsigned long owner_id, std::string placement, unsigned location, std::string container_placement = "", unsigned container_location = 0) const;

    // public accessors
    void                        ID(unsigned long ID)                { _ID = ID; }
    unsigned long               ID(void) const                      { return _ID; }
    void                        type(Type type);
    void                        type(Type type, ROW row);
    void                        type(Type type, void* extra_ptr);
    Type                        type(void) const                    { return _type; }
    void                        vnum(unsigned long vnum)            { _vnum = vnum; }
    unsigned long               vnum(void) const                    { return _vnum; }
    FlagBank&                   flags(void)                         { return _flags; }
    const FlagBank&             flags(void) const                   { return _flags; }
    Identifiers&                identifiers(void)                   { return _identifiers; }
    const Identifiers&          identifiers(void) const             { return _identifiers; }
    std::set<Compound*>&        composition(void)                   { return _composition; }
    const std::set<Compound*>&  composition(void) const             { return _composition; }
    void                        level(unsigned level)               { _level = level; }
    unsigned                    level(void) const                   { return _level; }
    void                        value(unsigned value)               { _value = value; }
    unsigned                    value(void) const                   { return _value; }
    void                        wearable(const Wearable& wearable)  { _wearable = wearable; }
    Wearable                    wearable(void) const                { return _wearable; }
    std::list<Modifier*>&       modifiers(void)                     { return _modifiers; }
    const std::list<Modifier*>& modifiers(void) const               { return _modifiers; }
    void                        extra(void* extra)                  { _extra = extra; }
    void*                       extra(void) const                   { return _extra; }

    // additional accessors
    bool  noremove(void) const  { return flags().test(OBJECT_NOREMOVE); }
    bool  nodrop(void) const    { return flags().test(OBJECT_NODROP); }
    bool  noget(void) const     { return flags().test(OBJECT_NOGET); }
    bool  meltdrop(void) const  { return flags().test(OBJECT_MELTDROP); }
    bool  humming(void) const   { return flags().test(OBJECT_HUMMING); }
    bool  glowing(void) const   { return flags().test(OBJECT_GLOWING); }
    bool  burnproof(void) const { return flags().test(OBJECT_BURNPROOF); }

  private:
    // data
    unsigned long         _ID;
    Type                  _type;
    unsigned long         _vnum;
    FlagBank              _flags;
    Identifiers           _identifiers;
    std::set<Compound*>   _composition;
    unsigned              _level;
    unsigned              _value;
    Wearable              _wearable;
    std::list<Modifier*>  _modifiers;
    void*                 _extra;

    // methods
    void  deleteExtra(void);
};

#endif // H_SYMPHONY_OBJECT
