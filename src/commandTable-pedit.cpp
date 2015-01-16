
#include "commands-pedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

PeditCommands::PeditCommands(void) {
  name("Pedit");
  add(new CmdExit(), true);
  add(new PCmdColor(), true);
  add(new PCmdHeadBuilder(), true);
  return;
}

