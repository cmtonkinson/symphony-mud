
#include "object-weapon.h"
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

ObjWeapon::ObjWeapon(ROW row) {
  type().set((int)row["weapon_type"]);
  verb().set((int)row["weapon_verb"]);
  damage().number(row["weapon_damage_number"]);
  damage().faces(row["weapon_damage_faces"]);
  return;
}

ObjWeapon::~ObjWeapon(void) {
  return;
}
