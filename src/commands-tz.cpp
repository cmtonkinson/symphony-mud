
#include "avatar.h"
#include "command-def.h"
#include "container.h"
#include "display.h"
#include "group.h"
#include "io-handler.h"
#include "room.h"
#include "world.h"
#include "zone.h"

CmdTell::CmdTell(void) {
  name("tell");
  playerOnly(true);
  addSyntax(0, "");
  addSyntax(-2, "<target> <message>");
  brief("Sends the message to the target on the Out-Of-Charater 'tell' channel.");
  return;
}

bool CmdTell::execute(Being* being, const std::vector<std::string>& args) {
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
    if (!(avatar()->canSee(target) == Being::SEE_NAME)) {
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

bool CmdTime::execute(Being* being, const std::vector<std::string>& args) {
  std::string output;
  output.append("Blah blah blah - haven't figured this out yet (but coming soon!)\n\n");
  being->send(output);
  return true;
}

CmdTimers::CmdTimers(void) {
  name("timers");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status on timers set to fire in the near future.");
  return;
}

bool CmdTimers::execute(Being* being, const std::vector<std::string>& args) {
  being->send("This command has been temporarily broken.");
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

bool CmdTitle::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)being;
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

CmdTrain::CmdTrain(void) {
  name("train");
  addSyntax(-1, "<ability>");
  brief("Spend 1 training point to increase your proficiency in an ability.");
  return;
}

bool CmdTrain::execute(Being* being, const std::vector<std::string>& args) {
  Ability* ability = nullptr;
  std::string name = args[0];
  double improvement = 0.0;
  int rounded = 0;

  if (being->trains() < 1) {
    being->send("You don't have any training points.");
    return false;
  }

  if ((ability = being->learned().find(name)) == nullptr) {
    being->send("Which ability did you want to train?");
    return false;
  }

  if (being->mastery(ability) == 100) {
    being->send("You've already mastered '{m%s{x.'", ability->name().c_str());
    return false;
  }

  // Increase mastery by X% of the remaining distance to mastery.
  // X is determined by the difficulty of the Ability.
  improvement = fmax(1.0, (100.0 - being->abilityMastery()[ability]) / ability->trainingFactor());
  rounded = ROUND_2_INT(improvement);

  being->abilityMastery()[ability] += rounded;
  being->trains(being->trains() - 1);
  being->send("You train your {m%s{x ability.\n", ability->name().c_str());
  being->send("You have {B%u{x training points remaining.", being->trains());
  return true;
}

CmdTutor::CmdTutor(void) {
  name("tutor");
  level(Being::GOD);
  addSyntax(1, "<player>");
  brief("Grant a player mastery of all abilities available to them.");
  return;
}

bool CmdTutor::execute(Being* being, const std::vector<std::string>& args) {
  Being* target = nullptr;

  if ((target = being->findBeing(args[0])) == NULL) {
    being->send("They're not around at the moment.");
    return false;
  }
  if (!being->canAlter(target)) {
    being->send("You can't tutor %s.", target->name());
    target->send("%s just tried to tutor you.", being->name());
    return false;
  }

  target->masterAllTheThings();
  target->send("%s has tutored you. You are overwhelmed with mastery.\n", being->name());
  being->send("You have tutored %s. They even LOOK smarter!\n", target->name());
  return true;
}

CmdUngroup::CmdUngroup(void) {
  name("ungroup");
  playerOnly(true);
  addSyntax(0, "");
  brief("Remove yourself from your current group.");
  return;
}

bool CmdUngroup::execute(Being* being, const std::vector<std::string>& args) {
  if (being->group()->members().size() == 1) {
    being->send("You're already solo.");
  } else {
    being->ungroup();
  }
  return true;
}

CmdUnlock::CmdUnlock(void) {
  name("unlock");
  addSyntax(1, "<direction>");
  brief("Unlocks a door.");
  return;
}

bool CmdUnlock::execute(Being* being, const std::vector<std::string>& args) {
  std::list<Item*> foo;
  Exit* exit = NULL;
  Item* key = NULL;

  if ((exit = being->room()->exit(args[0])) == NULL) {
    being->send("There is no door in that direction.");
    return false;
  }
  if (!exit->flag(EXIT_CLOSED)) {
    being->send("It's open.");
    return false;
  }
  if (!exit->flag(EXIT_LOCKED)) {
    being->send("It's already unlocked.");
    return false;
  }
  foo = being->inventory().searchItems(exit->key());
  if (!foo.empty()) {
    key = foo.front();
  }
  if (key == NULL || !key->isKey()) {
    being->send("You don't have the key.");
    return false;
  }

  exit->flag(EXIT_LOCKED, false);
  being->send("You unlock the door.");
  being->room()->send_cond("$p unlocked the way $e.", being, exit);
  return true;
}

CmdWear::CmdWear(void) {
  name("wear");
  addSyntax(1, "<item>");
  brief("Equips an item.");
  return;
}

bool CmdWear::execute(Being* being, const std::vector<std::string>& args) {
  std::list<Item*> items;
  Item* removed = NULL;
  std::string error;

  items = being->inventory().searchItems(args[0]);
  if (!items.empty()) {
    for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
      if (being->wear(*it, error, removed)) {
        being->inventory().remove(*it);
        if (removed) {
          being->send("You remove %s{x.\n", removed->identifiers().shortname().c_str());
          being->room()->send_cond("$p removes $o.\n", being, removed);
        }
        being->send("You wear %s{x.\n", (*it)->identifiers().shortname().c_str());
        being->room()->send_cond("$p wears $o.\n", being, *it);
      } else {
        being->send(error);
      }
    }
  } else {
    being->send("You don't have that.");
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

bool CmdWho::execute(Being* being, const std::vector<std::string>& args) {
  std::map<std::string,Avatar*>::const_iterator it;
  std::string output;
  std::string who;

  output.append("     Players Online\n");
  for (it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (it->second->isConnected() && being->canSee(it->second) == Being::SEE_NAME) {
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
  being->send(output);
  return true;
}

CmdWhoAmI::CmdWhoAmI(void) {
  name("whoami");
  addSyntax(0, "");
  brief("Displays your account name.");
  return;
}

bool CmdWhoAmI::execute(Being* being, const std::vector<std::string>& args) {
  being->send("You are logged in as %s.", being->identifiers().shortname().c_str());
  return true;
}

CmdWorldSave::CmdWorldSave(void) {
  name("worldsave");
  playerOnly(true);
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Saves the whole world! (but don't get cocky, it's not really that heroic)");
  return;
}

bool CmdWorldSave::execute(Being* being, const std::vector<std::string>& args) {
  World::Instance().save();
  avatar()->send("World zones and player profiles saved successfully.");
  return true;
}

CmdTest::CmdTest(void) {
  name("test");
  level(Being::CREATOR);
  addSyntax(0, "");
  brief("A command so that an admin can live-test arbitrary code fragments.");
  return;
}

/*
 * No, really.  This command exists soley so that a developer can live-test
 * arbitrary code fragments without exposing any half-baked Command logic
 * to the populace.
 */
bool CmdTest::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->send("No test code at the moment.");
  return true;
}
