/*
 * $Id: commands-pedit.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "area.h"
#include "avatar.h"
#include "commands-pedit.h"
#include "commandTable.h"
#include "commandTable-default.h"
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
