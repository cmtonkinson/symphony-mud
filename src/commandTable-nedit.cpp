
#include "commands-nedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

NeditCommands::NeditCommands(void) {
  name("Nedit");
  add(new CmdExit(), true);
  add(new NCmdAggressiveness(), true);
  add(new NCmdClass(), true);
  add(new NCmdDescription(), true);
  add(new NCmdGender(), true);
  add(new NCmdInformation(), true);
  add(new NCmdKeywords(), true);
  add(new NCmdLevel(), true);
  add(new NCmdLongname(), true);
  add(new NCmdMobility(), true);
  add(new NCmdRace(), true);
  add(new NCmdShortname(), true);
  return;
}
