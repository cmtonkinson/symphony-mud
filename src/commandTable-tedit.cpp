
#include "commands-tedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

TeditCommands::TeditCommands(void) {
  name("Text Editor");
  add(new CmdExit(), true);
  add(new TCmdAppend(), true);
  add(new TCmdCancel(), true);
  add(new TCmdClear(), true);
  add(new TCmdDelete(), true);
  add(new TCmdInsert(), true);
  add(new TCmdReplace(), true);
  add(new TCmdSend(), true);
  add(new TCmdShow(), true);
  return;
}
