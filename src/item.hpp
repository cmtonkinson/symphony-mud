
#ifndef H_SYMPHONY_ITEM
#define H_SYMPHONY_ITEM

#include <list>
#include <set>
#include "compound.hpp"
#include "flag-bank.hpp"
#include "flag-table.hpp"
#include "identifiers.hpp"
#include "modifier.hpp"

class ItemArmor;
class ItemClothing;
class ItemContainer;
class ItemFood;
class ItemFurniture;
class ItemJewelry;
class ItemKey;
class ItemTrash;
class ItemWeapon;

class Item {
  public:
    // constructors...
    Item(void);
    Item(const Item& ref);
    ~Item(void);

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

    ItemArmor*     armor(void) const       { return (ItemArmor*)_extra; }
    ItemClothing*  clothing(void) const    { return (ItemClothing*)_extra; }
    ItemContainer* container(void) const   { return (ItemContainer*)_extra; }
    ItemFood*      food(void) const        { return (ItemFood*)_extra; }
    ItemFurniture* furniture(void) const   { return (ItemFurniture*)_extra; }
    ItemJewelry*   jewelry(void) const     { return (ItemJewelry*)_extra; }
    ItemKey*       key(void) const         { return (ItemKey*)_extra; }
    ItemTrash*     trash(void) const       { return (ItemTrash*)_extra; }
    ItemWeapon*    weapon(void) const      { return (ItemWeapon*)_extra; }

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

    // shortcuts
    const char*   name(void) const  { return identifiers().shortname().c_str(); }

    // public accessors
    void                        ID(unsigned long ID)                { _ID = ID; }
    unsigned long               ID(void) const                      { return _ID; }
    void                        type(Type type);
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
    bool  noremove(void) const  { return flags().test(ITEM_NOREMOVE); }
    bool  nodrop(void) const    { return flags().test(ITEM_NODROP); }
    bool  noget(void) const     { return flags().test(ITEM_NOGET); }
    bool  meltdrop(void) const  { return flags().test(ITEM_MELTDROP); }
    bool  humming(void) const   { return flags().test(ITEM_HUMMING); }
    bool  glowing(void) const   { return flags().test(ITEM_GLOWING); }
    bool  burnproof(void) const { return flags().test(ITEM_BURNPROOF); }

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

#endif // H_SYMPHONY_ITEM
