
#include "commands-iedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

IeditCommands::IeditCommands(void) {
  name("Iedit");
  add(new CmdExit(), true);
  add(new ICmdAttribute(), true);
  add(new ICmdComposition(), true);
  add(new ICmdDelete(), true);
  add(new ICmdDescription(), true);
  add(new ICmdFlag(), true);
  add(new ICmdFurnitureCapacity(), true);
  add(new ICmdFurnitureLayOn(), true);
  add(new ICmdFurnitureSitAt(), true);
  add(new ICmdFurnitureSitOn(), true);
  add(new ICmdFurnitureStandOn(), true);
  add(new ICmdInformation(), true);
  add(new ICmdKeywords(), true);
  add(new ICmdLevel(), true);
  add(new ICmdLongname(), true);
  add(new ICmdShortname(), true);
  add(new ICmdType(), true);
  add(new ICmdValue(), true);
  add(new ICmdWeaponDamage(), true);
  add(new ICmdWeaponType(), true);
  add(new ICmdWeaponVerb(), true);
  add(new ICmdWearable(), true);
  return;
}
