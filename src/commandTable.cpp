
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
