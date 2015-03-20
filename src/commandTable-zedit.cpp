
#include "commands-zedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

ZeditCommands::ZeditCommands(void) {
  name("Zedit");
  add(new CmdExit(), true);
  add(new ZCmdDelete(), true);
  add(new ZCmdInformation(), true);
  add(new ZCmdName(), true);
  add(new ZCmdPermission(), true);
  add(new ZCmdTerrain(), true);
  return;
}

