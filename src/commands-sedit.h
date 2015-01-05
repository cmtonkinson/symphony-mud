/*
 * $Id: commands-sedit.h 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_COMMANDS_SEDIT
#define H_SYMPHONY_COMMANDS_SEDIT

#include "command.h"

DEF_COMMAND(SCmdAudible)
DEF_COMMAND(SCmdDelete)
DEF_COMMAND(SCmdInformation)
DEF_COMMAND(SCmdTactile)

DEF_COMMAND(SCmdTargetNone)
DEF_COMMAND(SCmdTargetSelf)
DEF_COMMAND(SCmdTargetVictim)

DEF_COMMAND(SCmdNoneActor)
DEF_COMMAND(SCmdNoneRoom)

DEF_COMMAND(SCmdSelfActor)
DEF_COMMAND(SCmdSelfRoom)

DEF_COMMAND(SCmdVictimActor)
DEF_COMMAND(SCmdVictimVictim)
DEF_COMMAND(SCmdVictimRoom)

#endif // #ifndef H_SYMPHONY_COMMANDS_SEDIT
