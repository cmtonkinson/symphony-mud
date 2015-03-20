
#include "item-weapon.h"
#include "regex.h"

ObjWeapon::ObjWeapon(void) {
  type().set(0);
  verb().set(0);
  return;
}

ObjWeapon::ObjWeapon(const ObjWeapon& ref):
    _type(ref.type()),
    _verb(ref.verb()),
    _damage(ref.damage()) {
  return;
}

ObjWeapon::~ObjWeapon(void) {
  return;
}
