/*
 * $Id: commandTable-sedit.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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
