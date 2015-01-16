
#include "area.h"
#include "avatar.h"
#include "commands-pedit.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "io-handler.h"
#include "room.h"
#include "world.h"

/*
PCmd::PCmd(void) {
  name("");
  level(CREATOR);
  addSyntax(0, "");
  brief("");
  return;
}

bool PCmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

PCmdColor::PCmdColor(void) {
  name("color");
  level(CREATOR);
  addSyntax(1, "<color code>");
  brief("Changes the message color of 'gecho'.");
  return;
}

bool PCmdColor::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = avatar()->pedit();
  target->gechoColor(args[0][0]);
  target->send("Your {%cgecho color {xhas been reset!", target->gechoColor());
  avatar()->send("You have reset %s's {%cgecho color{x!", target->identifiers().shortname().c_str(), target->gechoColor());
  return true;
}

PCmdHeadBuilder::PCmdHeadBuilder(void) {
  name("headbuilder");
  level(CREATOR);
  addSyntax(0, "");
  brief("Toggles whether the Player is marked as a 'Head Builder' or not.");
  return;
}

bool PCmdHeadBuilder::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = avatar()->pedit();
  if (target->level() < DEMIGOD) {
    avatar()->send("%s must be promoted at least to level %d before being granted Head Builder status.", target->identifiers().shortname().c_str(), DEMIGOD);
    return false;
  }
  if (target->adminFlags().toggle(ADMIN_HEADBUILDER)) {
    target->send("You have been granted Head Builder status.");
    avatar()->send("%s has been granted Head Builder status.", target->identifiers().shortname().c_str());
  } else {
    target->send("You no longer have Head Builder status.");
    avatar()->send("%s no longer has Head Builder status.", target->identifiers().shortname().c_str());
  }
  return true;
}
