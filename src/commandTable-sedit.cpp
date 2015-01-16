
#include "commands-sedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

SeditCommands::SeditCommands(void) {
  name("Social Editor");
  add(new CmdExit(), true);
  add(new SCmdAudible(), true);
  add(new SCmdDelete(), true);
  add(new SCmdInformation(), true);
  add(new SCmdTactile(), true);
  add(new SCmdTargetNone(), true);
  add(new SCmdTargetSelf(), true);
  add(new SCmdTargetVictim(), true);
  add(new SCmdNoneActor(), true);
  add(new SCmdNoneRoom(), true);
  add(new SCmdSelfActor(), true);
  add(new SCmdSelfRoom(), true);
  add(new SCmdVictimActor(), true);
  add(new SCmdVictimVictim(), true);
  add(new SCmdVictimRoom(), true);
  return;
}
