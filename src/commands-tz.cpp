
#include "area.h"
#include "avatar.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "container.h"
#include "display.h"
#include "group.h"
#include "io-handler.h"
#include "room.h"
#include "world.h"

CmdTell::CmdTell(void) {
  name("tell");
  playerOnly(true);
  addSyntax(0, "");
  addSyntax(-2, "<target> <message>");
  brief("Sends the message to the target on the Out-Of-Charater 'tell' channel.");
  return;
}

bool CmdTell::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  if (args[0].empty()) {
    avatar()->channelFlags().toggle(CHANNEL_TELL);
    if (avatar()->channelFlags().test(CHANNEL_TELL)) {
      avatar()->send("Tell channel [{GON{x]");
    } else {
      avatar()->send("Tell channel [{rOFF{x]");
    }
    return true;
  }

  if ((target = World::Instance().findAvatar(args[0])) == NULL) {
    avatar()->send("They're not around at the moment.");
    return false;
  } else {
    if (avatar() == target) {
      avatar()->send("That would be fairly pointless, no?");
      return false;
    }
    if (!(avatar()->canSee(target) == Creature::SEE_NAME)) {
      avatar()->send("They're not around at the moment.");
      return false;
    }
    if (!target->channelFlags().test(CHANNEL_TELL)) {
      avatar()->send("They're not receiving tells right now.");
      return false;
    }
    target->replyTo(avatar()->identifiers().shortname());
    target->send("%s tells you, \"{Y%s{x\"", target->seeName(avatar(), true).c_str(), args[1].c_str());
    avatar()->send("You tell %s, \"{Y%s{x\"", target->identifiers().shortname().c_str(), args[1].c_str());
  }

  return true;
}

CmdTime::CmdTime(void) {
  name("time");
  addSyntax(0, "");
  brief("Displays the current game time.");
  return;
}

bool CmdTime::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output;
  output.append("Blah blah blah - haven't figured this out yet (but coming soon!)\n\n");
  creature->send(output);
  return true;
}

CmdTimers::CmdTimers(void) {
  name("timers");
  level(Creature::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status on timers set to fire in the near future.");
  return;
}

bool CmdTimers::execute(Creature* creature, const std::vector<std::string>& args) {
  creature->send("This command has been temporarily broken.");
  return true;
}

CmdTitle::CmdTitle(void) {
  name("title");
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your player 'Who list' title.");
  return;
}

bool CmdTitle::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)creature;
  std::string title = args[0];

  if (title.empty()) {
    avatar->send("Current title: %s", avatar->title());
    return true;
  }

  if (title.compare("clear") == 0) {
    avatar->title("");
    avatar->send("Your title has been removed.");
    return true;
  }

  avatar->title(title);
  avatar->send("Your new title is: %s{x", avatar->title());
  return true;
}

CmdUngroup::CmdUngroup(void) {
  name("ungroup");
  playerOnly(true);
  addSyntax(0, "");
  brief("Remove yourself from your current group.");
  return;
}

bool CmdUngroup::execute(Creature* creature, const std::vector<std::string>& args) {
  if (creature->group()->members().size() == 1) {
    creature->send("You're already solo.");
  } else {
    creature->ungroup();
  }
  return true;
}

CmdUnlock::CmdUnlock(void) {
  name("unlock");
  addSyntax(1, "<direction>");
  brief("Unlocks a door.");
  return;
}

bool CmdUnlock::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> foo;
  Exit* exit = NULL;
  Object* key = NULL;

  if ((exit = creature->room()->exit(args[0])) == NULL) {
    creature->send("There is no door in that direction.");
    return false;
  }
  if (!exit->flag(EXIT_CLOSED)) {
    creature->send("It's open.");
    return false;
  }
  if (!exit->flag(EXIT_LOCKED)) {
    creature->send("It's already unlocked.");
    return false;
  }
  foo = creature->inventory().searchObjects(exit->key());
  if (!foo.empty()) {
    key = foo.front();
  }
  if (key == NULL || !key->isKey()) {
    creature->send("You don't have the key.");
    return false;
  }

  exit->flag(EXIT_LOCKED, false);
  creature->send("You unlock the door.");
  creature->room()->send_cond("$p unlocked the way $e.", creature, exit);
  return true;
}

CmdWear::CmdWear(void) {
  name("wear");
  addSyntax(1, "<object>");
  brief("Equips an object.");
  return;
}

bool CmdWear::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> objects;
  Object* removed = NULL;
  std::string error;

  objects = creature->inventory().searchObjects(args[0]);
  if (!objects.empty()) {
    for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
      if (creature->wear(*it, error, removed)) {
        creature->inventory().remove(*it);
        if (removed) {
          creature->send("You remove %s{x.\n", removed->identifiers().shortname().c_str());
          creature->room()->send_cond("$p removes $o.\n", creature, removed);
        }
        creature->send("You wear %s{x.\n", (*it)->identifiers().shortname().c_str());
        creature->room()->send_cond("$p wears $o.\n", creature, *it);
      } else {
        creature->send(error);
      }
    }
  } else {
    creature->send("You don't have that.");
    return false;
  }

  return true;
}

CmdWho::CmdWho(void) {
  name("who");
  addSyntax(0, "");
  brief("Display names of players who are currently connected.");
  return;
}

bool CmdWho::execute(Creature* creature, const std::vector<std::string>& args) {
  std::map<std::string,Avatar*>::const_iterator it;
  std::string output;
  std::string who;

  output.append("     Players Online\n");
  for (it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (it->second->isConnected() && creature->canSee(it->second) == Creature::SEE_NAME) {
      output.append("  ");
      if (it->second->adminFlags().test(ADMIN_INCOGNITO)) {
        output.append("{WINVIS{x ");
      }
      who = it->second->listWhoFlags();
      if (!who.empty()) {
        output.append(who).append(1, ' ');
      }
      output.append(it->second->identifiers().shortname());
      output.append(it->second->title());
      output.append("{x\n");
    }
  }
  creature->send(output);
  return true;
}

CmdWhoAmI::CmdWhoAmI(void) {
  name("whoami");
  addSyntax(0, "");
  brief("Displays your account name.");
  return;
}

bool CmdWhoAmI::execute(Creature* creature, const std::vector<std::string>& args) {
  creature->send("You are logged in as %s.", creature->identifiers().shortname().c_str());
  return true;
}

CmdWorldSave::CmdWorldSave(void) {
  name("worldsave");
  playerOnly(true);
  level(Creature::DEMIGOD);
  addSyntax(0, "");
  brief("Saves the whole world! (but don't get cocky, it's not really that heroic)");
  return;
}

bool CmdWorldSave::execute(Creature* creature, const std::vector<std::string>& args) {
  World::Instance().save();
  avatar()->send("World areas and player profiles saved successfully.");
  return true;
}

CmdTest::CmdTest(void) {
  name("test");
  level(Creature::CREATOR);
  addSyntax(0, "");
  brief("A command so that an admin can live-test arbitrary code fragments.");
  return;
}

/*
 * No, really.  This command exists soley so that a developer can live-test
 * arbitrary code fragments without exposing any half-baked Command logic
 * to the populace.
 */
#include "storage.h"
#include <fstream>
bool CmdTest::execute(Creature* creature, const std::vector<std::string>& args) {
  Room* r         = nullptr;
  unsigned status = 0;

  FILE* fp = fopen("foo.txt", "w");
  Storage::dump(fp, creature->room());
  fclose(fp);
  std::ifstream ifs("foo.txt");
  std::string content( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped...\n");
  creature->send(content);

  creature->send("\nLoaded...\n");
  fp = fopen("foo.txt", "r");
  while (!feof(fp)) {
    r = new Room();
    status = Storage::load(fp, r);
    if (status) {
      creature->send("Loaded Room(%x)::vnum        = %lu\n", r, r->vnum());
      creature->send("Loaded Room(%x)::name        = %s\n", r, r->name().c_str());
      creature->send("Loaded Room(%x)::description = %s\n", r, r->description().c_str());
      creature->send("Loaded Room(%x)::smell       = %s\n", r, r->smell().c_str());
      creature->send("Loaded Room(%x)::sound       = %s\n", r, r->sound().c_str());
      creature->send("Loaded Room(%x)::terrain     = %s\n", r, r->terrain()->name().c_str());
      for (unsigned u = 0; u < 6; ++u) {
        if (r->exit(u) != nullptr) {
          creature->send("Room(%x)::Exit(%x)::direction = %u\n", r, r->exit(u), r->exit(u)->direction().number());
          creature->send("Room(%x)::Exit(%x)::key       = %lu\n", r, r->exit(u), r->exit(u)->key());
        }
      }
    }
  }
  fclose(fp);

  // avatar()->send("No test code at the moment.");
  return true;
}
