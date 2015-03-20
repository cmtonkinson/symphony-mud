
#include "command.h"
#include "commandTable.h"
#include "display.h"
#include "regex.h"
#include "room.h"
#include "socket.h"
#include "storage.h"
#include "world.h"

///////////////////////////////////////////// BASE CLASS /////////////////////////////////////////////
Command::Command(void) {
  enabled(true);
  playerOnly(false);
  special(false);
  social(false);
  level(1);
  allowedPositions(~0);
  commandTable(NULL);
  avatar(NULL);
  return;
}

Command::~Command(void) {
  return;
}

void Command::level(const unsigned short& level) {
  _level = level;
  if (_level > Being::HERO) {
    playerOnly(true);
  }
  return;
}

void Command::addSyntax(const short& num_args, std::string usage) {
  arguments().insert(num_args);
  syntax().push_back(usage);
  return;
}

const char* Command::printSyntax(void) const {
  std::string dest;
  bool first = true;
  dest.assign("{g").append(name()).append("{x is a level {g").append(estring(level())).append("{x command.\n\n");
  dest.append("{gSynopsis:{x ").append(brief()).append("\n");
  for (std::vector<std::string>::const_iterator it = syntax().begin(); it != syntax().end(); ++it) {
    if (first) {
      dest.append("{gUsage:{x ").append(name()).append(1, ' ').append(*it).append(1, '\n');
      first = false;
    } else {
      dest.append("       ").append(name()).append(1, ' ').append(*it).append(1, '\n');
    }
  }
  if (!options().empty()) {
    dest.append(1, '\n');
  }
  for (OptionMap::const_iterator it = options().begin(); it != options().end(); ++it) {
    dest.append("Options for {g").append(it->first).append("{x: ").append(it->second).append(1, '\n');
  }
  if (!_seeAlso.empty()) {
    dest.append("\n{gSee also:{x ").append(Regex::implode(", ", _seeAlso)).append("\n");
  }
  return dest.c_str();
}

void Command::addOptions(std::string argument, std::string option_list) {
  options().insert(std::make_pair(argument, option_list));
  return;
}

void Command::seeAlso(std::string command) {
  _seeAlso.insert(command);
  return;
}

///////////////////////////////////////////// SOCIALS /////////////////////////////////////////////
SocialCommand::SocialCommand(void) {
  social(true);
  return;
}

SocialCommand::SocialCommand(std::string name_, std::string creator_) {
  social(true);
  name(name_);
  creator(creator_);

  targetNone(false);
  targetSelf(false);
  targetVictim(false);
  noneActor("You do something.");
  noneRoom("$c does something.");
  selfActor("You do something to yourself.");
  selfRoom("$c does something to $f.");
  victimActor("You do something to $C.");
  victimVictim("$c does something to you.");
  victimRoom("$c does something to $C.");

  Commands::Instance().add(this);
  return;
}

SocialCommand::~SocialCommand(void) {
  return;
}

void SocialCommand::initialize(void) {
  // Add default argument counts.
  if (targetNone()) arguments().insert(0);
  if (targetSelf() || targetVictim()) arguments().insert(1);
  // Register with the global table.
  Commands::Instance().add(this);
  return;
}

void SocialCommand::destroy(void) {
  std::vector<Command*> commands = Commands::Instance().commands();
  std::vector<Command*>::iterator iter;

  // Delete the data file.
  remove(Storage::filename(this).c_str());

  // Remove the Social from the CommandTable.
  for (iter = commands.begin(); iter != commands.end(); ++iter) {
    if (!(*iter)->social()) continue;
    if (dynamic_cast<SocialCommand*>(*iter) == this) {
      Commands::Instance().commands().erase(iter);
      break;
    }
  }

  // Self-delete.
  delete this;
  return;
}

void SocialCommand::save(void) {
  std::string filename = Storage::filename(this);
  FILE* fp             = nullptr;

  if ((fp = fopen(filename.c_str(), "w")) != NULL) {
    Storage::dump(fp, this);
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to write social file %s.\n", filename.c_str());
  }

  return;
}

SocialCommand* SocialCommand::load(std::string filename) {
  SocialCommand* social = nullptr;
  FILE* fp              = nullptr;

  if ((fp = fopen(filename.c_str(), "r")) != NULL) {
    social = new SocialCommand();
    Storage::load(fp, social);
    social->initialize();
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to read social file %s.\n", filename.c_str());
  }

  return social;
}

bool SocialCommand::execute(Being* being, const std::vector<std::string>& args) {
  Being* victim = NULL;

  if (args[0].empty()) {
    if (targetNone()) {
      being->send(noneActor());
      being->room()->send_cond(noneRoom().c_str(), being, being, NULL, Room::TO_ROOM, isAudible());
      return true;
    }
  } else if (args.size() == 1) {
    victim = being->findBeing(args[0]);
    if (victim == NULL) {
      being->send("They're not around at the moment.");
      return false;
    }
    if (victim == being) {
      if (targetSelf()) {
        being->send(selfActor());
        being->room()->send_cond(selfRoom().c_str(), being, being, being, Room::TO_ROOM, isAudible());
        return true;
      } else {
        being->send("You'd look pretty silly doing that to yourself.");
        return false;
      }
    } else {
      if (targetVictim()) {
        being->send(Display::formatAction(victimActor().c_str(), being, being, victim, being));
        // Unless the social is audible or tactile, the victim shouldn't see it if they can't see the actor...
        if (victim->canSee(being) > Being::SEE_NOTHING || isAudible() || isTactile()) {
          victim->send(Display::formatAction(victimVictim().c_str(), being, being, victim, victim));
        }
        being->room()->send_cond(victimRoom().c_str(), being, being, victim, Room::TO_NOTVICT, isAudible());
        return true;
      } else {
        being->send("You can't do that to someone else.");
        return false;
      }
    }
  }

  being->send("social error");
  return false;
}
