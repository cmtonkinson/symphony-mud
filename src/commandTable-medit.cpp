
#include "commands-medit.h"
#include "commandTable.h"
#include "commandTable-default.h"

MeditCommands::MeditCommands(void) {
  name("Medit");
  add(new CmdExit(), true);
  add(new MCmdAggressiveness(), true);
  add(new MCmdClass(), true);
  add(new MCmdDescription(), true);
  add(new MCmdGender(), true);
  add(new MCmdInformation(), true);
  add(new MCmdKeywords(), true);
  add(new MCmdLevel(), true);
  add(new MCmdLongname(), true);
  add(new MCmdMobility(), true);
  add(new MCmdRace(), true);
  add(new MCmdShortname(), true);
  return;
}
