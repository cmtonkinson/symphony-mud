
#include "commands-aedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

AeditCommands::AeditCommands(void) {
  name("Aedit");
  add(new CmdExit(), true);
  add(new ACmdDelete(), true);
  add(new ACmdInformation(), true);
  add(new ACmdName(), true);
  add(new ACmdPermission(), true);
  add(new ACmdTerrain(), true);
  return;
}

