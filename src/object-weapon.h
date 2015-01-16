
#ifndef H_SYMPHONY_OBJECT_WEAPON
#define H_SYMPHONY_OBJECT_WEAPON

#include "dice.h"
#include "enumTable.h"
#include "flagTable.h"
#include "mysql.h"

class ObjWeapon {
  public:
    // constructors
    ObjWeapon(void);
    ObjWeapon(const ObjWeapon& ref);
    ObjWeapon(ROW row);
    ~ObjWeapon(void);

    // public accessors
    WeaponType&       type(void)                  { return _type; }
    const WeaponType& type(void) const            { return _type; }
    DamageVerb&       verb(void)                  { return _verb; }
    const DamageVerb& verb(void) const            { return _verb; }
    FlagBank&         flags(void)                 { return _flags; }
    const FlagBank&   flags(void) const           { return _flags; }
    Dice&             damage(void)                { return _damage; }
    const Dice&       damage(void) const          { return _damage; }

  private:
    WeaponType  _type;
    DamageVerb  _verb;
    FlagBank    _flags;
    Dice        _damage;
};

#endif // H_SYMPHONY_OBJECT_WEAPON
