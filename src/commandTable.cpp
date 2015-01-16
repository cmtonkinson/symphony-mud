/*
 * $Id: commandTable.cpp 369 2010-05-31 15:48:57Z cmtonkinson@gmail.com $
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

#include "command.h"
#include "commandTable.h"
#include "creature.h"
#include "regex.h"

CommandTable::~CommandTable(void) {
  return;
}

void CommandTable::add(Command* command, bool special) {
  command->commandTable(this);
  commands().push_back(command);
  if (command->level() > HERO) {
    command->playerOnly(true);
  }
  if (special) {
    command->special(true);
    command->playerOnly(true);
  }
  return;
}

Command* CommandTable::find(std::string& args) {
  std::vector<Command*>::iterator it;
  Command* command = NULL;
  std::string name = args.substr(0, args.find_first_of(' '));

  // Check for full command names and shortcuts...
  for (it = commands().begin(); it != commands().end(); ++it) {
    if (name == (*it)->name() || name == (*it)->shortcut()) {
      command = *it;
      break;
    }
  }

  // If we didn't find a match using full names, check the prefixes...
  if (!command) {
    for (it = commands().begin(); it != commands().end(); ++it) {
      if (Regex::strPrefix(name, (*it)->name())) {
        command = *it;
        break;
      }
    }
  }

  args = Regex::trim(args.substr(name.size()));
  return command;
}

Command* CommandTable::find(const std::string& args) {
  std::string name = args;
  return find(name);
}
