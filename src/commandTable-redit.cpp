
#include "commands-redit.h"
#include "commandTable.h"
#include "commandTable-default.h"

ReditCommands::ReditCommands(void) {
  name("Redit");
  add(new CmdExit(), true);
  add(new RCmdCreate(), true);
  add(new RCmdDelete(), true);
  add(new RCmdDescription(), true);
  add(new RCmdDig(), true);
  add(new RCmdExitflag(), true);
  add(new RCmdFlag(), true);
  add(new RCmdInformation(), true);
  add(new RCmdKey(), true);
  add(new RCmdLink(), true);
  add(new RCmdName(), true);
  add(new RCmdSmell(), true);
  add(new RCmdSound(), true);
  add(new RCmdTerrain(), true);
  add(new RCmdUnlink(), true);
  return;
}

