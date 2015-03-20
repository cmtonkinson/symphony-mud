
#ifndef H_SYMPHONY_ITEM_WEAPON
#define H_SYMPHONY_ITEM_WEAPON

#include "dice.h"
#include "enumTable.h"

class ItemWeapon {
  public:
    // constructors
    ItemWeapon(void);
    ItemWeapon(const ItemWeapon& ref);
    ~ItemWeapon(void);

    // public accessors
    WeaponType&       type(void)                  { return _type; }
    const WeaponType& type(void) const            { return _type; }
    DamageVerb&       verb(void)                  { return _verb; }
    const DamageVerb& verb(void) const            { return _verb; }
    Dice&             damage(void)                { return _damage; }
    const Dice&       damage(void) const          { return _damage; }

  private:
    WeaponType  _type;
    DamageVerb  _verb;
    Dice        _damage;
};

#endif // H_SYMPHONY_ITEM_WEAPON
