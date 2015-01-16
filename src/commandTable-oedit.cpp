
#include "commands-oedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

OeditCommands::OeditCommands(void) {
  name("Oedit");
  add(new CmdExit(), true);
  add(new OCmdAttribute(), true);
  add(new OCmdComposition(), true);
  add(new OCmdDelete(), true);
  add(new OCmdDescription(), true);
  add(new OCmdFlag(), true);
  add(new OCmdFurnitureCapacity(), true);
  add(new OCmdFurnitureLayOn(), true);
  add(new OCmdFurnitureSitAt(), true);
  add(new OCmdFurnitureSitOn(), true);
  add(new OCmdFurnitureStandOn(), true);
  add(new OCmdInformation(), true);
  add(new OCmdKeywords(), true);
  add(new OCmdLevel(), true);
  add(new OCmdLongname(), true);
  add(new OCmdShortname(), true);
  add(new OCmdType(), true);
  add(new OCmdValue(), true);
  add(new OCmdWeaponDamage(), true);
  add(new OCmdWeaponType(), true);
  add(new OCmdWeaponVerb(), true);
  add(new OCmdWearable(), true);
  return;
}
