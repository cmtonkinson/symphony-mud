
#ifndef H_SYMPHONY_ITEM_TYPES
#define H_SYMPHONY_ITEM_TYPES

#include "dice.hpp"
#include "enum-table.hpp"
#include "inventory-container.hpp"

class Being;

///////////////////////////////////////////////////////////////////////////////
// ARMOR
///////////////////////////////////////////////////////////////////////////////
class ItemArmor {
  public:
    ItemArmor(const Item* base);
    ItemArmor(const Item* base, const ItemArmor& ref);
    ~ItemArmor(void);

    void              base(const Item* base)  { _base = base; }
    const Item*       base(void)              { return _base; }
    ArmorType&        type(void)              { return _type; }
    const ArmorType&  type(void) const        { return _type; }

  private:
    const Item* _base;
    ArmorType   _type;

};

///////////////////////////////////////////////////////////////////////////////
// CLOTHING
///////////////////////////////////////////////////////////////////////////////
class ItemClothing {
  public:
    ItemClothing(void)                      { return; }
    ItemClothing(const ItemClothing& ref)   { return; }
    ~ItemClothing(void)                     { return; }
};

///////////////////////////////////////////////////////////////////////////////
// CONTAINER
///////////////////////////////////////////////////////////////////////////////
class ItemContainer {
  public:
    ItemContainer(void): _inventory(&Identifiers::shortname)              { return; }
    ItemContainer(const ItemContainer& ref): _inventory(ref.inventory())  { return; }
    ~ItemContainer(void)                                                  { return; }

    // accessor methods...
    InventoryContainer&       inventory(void)         { return _inventory; }
    const InventoryContainer& inventory(void) const   { return _inventory; }

  private:
    InventoryContainer        _inventory;
};

///////////////////////////////////////////////////////////////////////////////
// FOOD
///////////////////////////////////////////////////////////////////////////////
class ItemFood {
  public:
    ItemFood(void)                  { return; }
    ItemFood(const ItemFood& ref)   { return; }
    ~ItemFood(void)                 { return; }
};

///////////////////////////////////////////////////////////////////////////////
// FURNITURE
///////////////////////////////////////////////////////////////////////////////
class ItemFurniture {
  public:
    ItemFurniture(void);
    ItemFurniture(const ItemFurniture& ref);
    ~ItemFurniture(void);

    // public accessors
    void      capacity(const unsigned& capacity)      { _capacity = capacity; }
    unsigned  capacity(void) const                    { return _capacity; }
    void      layOn(const unsigned& layOn)            { _layOn = layOn; }
    unsigned  layOn(void) const                       { return _layOn; }
    void      sitAt(const unsigned& sitAt)            { _sitAt = sitAt; }
    unsigned  sitAt(void) const                       { return _sitAt; }
    void      sitOn(const unsigned& sitOn)            { _sitOn = sitOn; }
    unsigned  sitOn(void) const                       { return _sitOn; }
    void      standOn(const unsigned& standOn)        { _standOn = standOn; }
    unsigned  standOn(void) const                     { return _standOn; }
    void      current(const unsigned& current)        { _current = current; }
    unsigned  current(void) const                     { return _current; }
    std::map<EnumInt,std::list<Being*> >&        beings(void)       { return _beings; }
    const std::map<EnumInt,std::list<Being*> >&  beings(void) const { return _beings; }

    // general methods
    bool  add(Being* being, const EnumInt& position);
    bool  remove(Being* being);

  private:
    unsigned _capacity; // how many, in total, can be using this at once?
    unsigned _layOn;    // how many can lay on this?
    unsigned _sitAt;    // how many can sit at this?
    unsigned _sitOn;    // how many can sit on this?
    unsigned _standOn;  // how many can stand on this?
    unsigned _current;  // how many, in total, are currently here?
    std::map<EnumInt,std::list<Being*> > _beings; // who's here?
};

///////////////////////////////////////////////////////////////////////////////
// JEWELRY
///////////////////////////////////////////////////////////////////////////////
class ItemJewelry {
  public:
    ItemJewelry(void)                     { return; }
    ItemJewelry(const ItemJewelry& ref)   { return; }
    ~ItemJewelry(void)                    { return; }
};

///////////////////////////////////////////////////////////////////////////////
// KEY
///////////////////////////////////////////////////////////////////////////////
class ItemKey {
  public:
    ItemKey(void)                 { return; }
    ItemKey(const ItemKey& ref)   { return; }
    ~ItemKey(void)                { return; }
};

///////////////////////////////////////////////////////////////////////////////
// TRASH
///////////////////////////////////////////////////////////////////////////////
class ItemTrash {
  public:
    ItemTrash(void)                   { return; }
    ItemTrash(const ItemTrash& ref)   { return; }
    ~ItemTrash(void)                  { return; }
};

///////////////////////////////////////////////////////////////////////////////
// WEAPON
///////////////////////////////////////////////////////////////////////////////
class ItemWeapon {
  public:
    ItemWeapon(const Item* base);
    ItemWeapon(const Item* base, const ItemWeapon& ref);
    ~ItemWeapon(void);

    // public accessors
    WeaponType&       type(void)                  { return _type; }
    const WeaponType& type(void) const            { return _type; }
    DamageVerb&       verb(void)                  { return _verb; }
    const DamageVerb& verb(void) const            { return _verb; }
    Dice&             damage(void)                { return _damage; }
    const Dice&       damage(void) const          { return _damage; }

    void              base(const Item* base)      { _base = base; }
    const Item*       base(void) const            { return _base; }
    void              keyStat(unsigned stat)      { _key_stat = stat; }
    unsigned          keyStat(void) const         { return _key_stat; }

    // random methods
    double weightFactor(void) const;

    // Return a double corresponding to the relative size/weight of the given
    // ItemWeapon. Returns between 1.0 and 5.0, with 1.0 being "small" and
    // 5.0 being "large."
    static double relativeSize(const ItemWeapon* weapon);

    // Return a double corresponding to the relative range of the given
    // ItemWeapon. Returns between 1.0 and 5.0 with 1.0 being "short" range
    // and 5.0 being "long range."
    static double relativeRange(const ItemWeapon* weapon);

  private:
    WeaponType  _type;
    DamageVerb  _verb;
    Dice        _damage;
    const Item* _base;
    unsigned    _key_stat;
};

#endif // #ifndef H_SYMPHONY_ITEM_TYPES
