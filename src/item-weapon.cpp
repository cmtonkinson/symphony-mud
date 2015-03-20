
#include "item-weapon.h"
#include "regex.h"

ItemWeapon::ItemWeapon(void) {
  type().set(0);
  verb().set(0);
  return;
}

ItemWeapon::ItemWeapon(const ItemWeapon& ref):
    _type(ref.type()),
    _verb(ref.verb()),
    _damage(ref.damage()) {
  return;
}

ItemWeapon::~ItemWeapon(void) {
  return;
}
