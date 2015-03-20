
#include "area.h"
#include "avatar.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "io-handler.h"
#include "item-container.h"
#include "room.h"
#include "world.h"

CmdNote::CmdNote(void) {
  name("note");
  playerOnly(true);
  addSyntax(1, "write");
  addSyntax(2, "read <number>");
  brief("Manipulates notes in the board system.");
  return;
}

bool CmdNote::execute(Being* being, const std::vector<std::string>& args) {
  std::map<unsigned,Board*>::iterator b_it;
  std::map<unsigned,Note*>::iterator n_it;
  Board* board = NULL;
  Note* note = NULL;

  if ((b_it = World::Instance().boards().find(avatar()->board())) == World::Instance().boards().end()) {
    avatar()->send("You haven't selected a board yet.");
    return false;
  } else {
    board = b_it->second;
  }

  if (args.size() == 1) {
    if (Regex::strPrefix(args[0], "write")) {
      avatar()->note(new Note(board->number(), avatar()->name()));
      avatar()->pushIOHandler(new NoteGetSubjectIOHandler(avatar()));
      return true;
    }
  }

  if (args.size() == 2) {
    if (Regex::strPrefix(args[0], "read")) {
      if ((n_it = board->notes().find(estring(args[1]))) == board->notes().end()) {
        avatar()->send("Sorry, that note doesn't exist.");
        return false;
      } else {
        note = n_it->second;
        avatar()->send("Author: {y%s{x\n", note->author().c_str());
        avatar()->send("Subject: {Y%s{x\n", note->subject().c_str());
        avatar()->send("-------------------------------------------------------------------------------\n");
        avatar()->send(note->body());
        if (*(note->body().end()-1) != '\n') {
          avatar()->send("\n");
        }
        avatar()->send("-------------------------------------------------------------------------------\n");
        return true;
      }
    }
  }

  avatar()->send(printSyntax());
  return false;
}

CmdOoc::CmdOoc(void) {
  name("ooc");
  playerOnly(true);
  addSyntax(0, "");
  addSyntax(-1, "<message>");
  brief("The Out-Of-Character channel.");
  return;
}

bool CmdOoc::execute(Being* being, const std::vector<std::string>& args) {
  if (args[0].empty()) {
    avatar()->channelFlags().toggle(CHANNEL_OOC);
    if (avatar()->channelFlags().test(CHANNEL_OOC)) {
      avatar()->send("OOC channel [{GON{x]");
    } else {
      avatar()->send("OOC channel [{rOFF{x]");
    }
    return true;
  }

  if (!avatar()->channelFlags().test(CHANNEL_OOC)) {
    avatar()->send("You have the OOC channel off at the moment.");
    return false;
  }

  for (std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (it->second->isConnected() && it->second != avatar()) {
      if (it->second->channelFlags().test(CHANNEL_OOC)) {
        it->second->send("%s OOC's{x \"{W%s{x\"", it->second->seeName(avatar(), true).c_str(), args[0].c_str());
      }
    }
  }
  avatar()->send("You OOC{x \"{W%s{x\"", args[0].c_str());

  return true;
}

CmdOpen::CmdOpen(void) {
  name("open");
  addSyntax(1, "<direction>");
  brief("Opens a door.");
  return;
}

bool CmdOpen::execute(Being* being, const std::vector<std::string>& args) {
  Exit* exit = NULL;
  Exit* inv = NULL;

  if ((exit = being->room()->exit(args[0])) == NULL) {
    being->send("There is no door in that direction.");
    return false;
  }

  if (!exit->flag(EXIT_CLOSED)) {
    being->send("It's already open.");
    return false;
  }

  if (exit->flag(EXIT_LOCKED)) {
    being->send("It's locked.");
    return false;
  }

  exit->flag(EXIT_CLOSED, false);
  being->send("You open the door.");
  being->room()->send("$p opened the way $e.", being, exit);
  if ((inv = exit->targetRoom()->exit(Exit::inverse(exit->direction().number()))) != NULL) {
    exit->targetRoom()->send("The way $e has been opened.", NULL, inv);
  }

  return true;
}

CmdPassword::CmdPassword(void) {
  name("password");
  addSyntax(3, "<old pw> <new pw> <confirm new pw>");
  brief("Changes your account password.");
  playerOnly(true);
  return;
}

bool CmdPassword::execute(Being* being, const std::vector<std::string>& args) {

  if (args[1].compare(args[2]) != 0) {
    being->send("The password you entered does not match the confirmation.");
    return false;
  }

  avatar()->setPassword(args[1]);
  avatar()->save();
  avatar()->send("Password updated (your profile has been saved for you automatically).");
  return true;
}

CmdPeace::CmdPeace(void) {
  name("peace");
  playerOnly(true);
  level(Being::GOD);
  addSyntax(0, "");
  addSyntax(1, "all");
  brief("Ends all combat, in the current room, or the whole world.");
  return;
}

bool CmdPeace::execute(Being* being, const std::vector<std::string>& args) {
  if (args[0].empty()) {
    for (std::list<Being*>::iterator iter = avatar()->room()->beings().begin(); iter != avatar()->room()->beings().end(); ++iter) {
      (*iter)->peace();
    }
    avatar()->room()->send_cond("$p has imposed peace here.\n", being);
    avatar()->send("You have imposed peace here.\n");
  } else {
    for (std::set<Being*>::iterator iter = World::Instance().getBeings().begin(); iter != World::Instance().getBeings().end(); ++iter) {
      (*iter)->peace();
      if (*iter == being) continue;
      (*iter)->send((*iter)->seeName(avatar(), true));
      (*iter)->send(" has imposed peace throughout the realm.\n");
    }
    avatar()->send("You have imposed peace throughout the realm.\n");
  }
  return true;
}

CmdPedit::CmdPedit(void) {
  name("pedit");
  level(Being::GOD);
  addSyntax(1, "<player>       (must be connected)");
  brief("Invokes the Player Editor.");
  return;
}

bool CmdPedit::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  // Find the player...
  if ((target = World::Instance().findAvatar(args[0])) == NULL) {
    avatar()->send("That player isn't around.");
    return false;
  }

  // Make sure noone else is in Pedit mode for the player...
  // Make sure no one else is editing the item...
  for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
    if (a_it->second->mode().number() == MODE_PEDIT && a_it->second->pedit() == target) {
      avatar()->send("Sorry, %s is currently editing %s.", avatar()->seeName(((Being*)a_it->second)).c_str(), target->identifiers().shortname().c_str());
      return false;
    }
  }

  if (avatar()->canAlter(target) || avatar()->level() >= Being::CREATOR) {
    avatar()->send("You're editing %s.", target->identifiers().shortname().c_str());
    avatar()->pedit(target);
    avatar()->pushIOHandler(new PeditIOHandler(avatar()));
    return true;
  } else {
    avatar()->send("You can't edit %s.", target->identifiers().shortname().c_str());
    return false;
  }

  return true;
}

CmdPoofin::CmdPoofin(void) {
  name("poofin");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your Poof-In.");
  return;
}

bool CmdPoofin::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)being;

  if (args[0].empty()) {
    avatar->send("Current poofin: %s", avatar->poofin());
    return true;
  }

  if (args[0].compare("clear") == 0) {
    avatar->poofin("");
    avatar->send("Your poofin has been removed.");
    return true;
  }

  if (Regex::lower(args[0]).find(Regex::lower(avatar->identifiers().shortname())) == std::string::npos) {
    avatar->send("Your poofin must contain your name.");
    return false;
  }

  avatar->poofin(args[0]);
  avatar->send("Your new poofin is: %s", avatar->poofin());

  return true;
}

CmdPoofout::CmdPoofout(void) {
  name("poofout");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your Poof-Out.");
  return;
}

bool CmdPoofout::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)being;

  if (args[0].empty()) {
    avatar->send("Current poofout: %s", avatar->poofout());
    return true;
  }

  if (args[0].compare("clear") == 0) {
    avatar->poofout("");
    avatar->send("Your poofout has been removed.");
    return true;
  }

  if (Regex::lower(args[0]).find(Regex::lower(avatar->identifiers().shortname())) == std::string::npos) {
    avatar->send("Your poofout must contain your name.");
    return false;
  }

  avatar->poofout(args[0]);
  avatar->send("Your new poofout is: %s", avatar->poofout());

  return true;
}

CmdPromote::CmdPromote(void) {
  name("promote");
  addSyntax(2, "<player> <level>");
  addSyntax(2, "<mob> <level>");
  level(Being::CREATOR);
  brief("Advances the target to the given level.");
  return;
}

bool CmdPromote::execute(Being* being, const std::vector<std::string>& args) {
  Being* target = NULL;
  unsigned level = estring(args[1]);

  // do the checks...
  if ((target = avatar()->findBeing(args[0])) == NULL) {
    avatar()->send("They're not around at the moment.");
    return false;
  }
  if (!avatar()->canAlter(target)) {
    avatar()->send("You can't promote %s.", target->identifiers().shortname().c_str());
    target->send("%s just tried to promote you to level %d.", avatar()->identifiers().shortname().c_str(), level);
    return false;
  }
  if (level < target->level()) {
    avatar()->send("I think you mean to demote them instead...");
    return false;
  }
  if (level == target->level()) {
    avatar()->send("That wouldn't really help %s much, now would it?", target->identifiers().shortname().c_str());
    return false;
  }
  if (level >= (unsigned)avatar()->level() - Being::ALTERABILITY_LEVEL_DIFFERENCE) {
    avatar()->send("You can't promote anyone higher than level %d.", avatar()->level() - (Being::ALTERABILITY_LEVEL_DIFFERENCE-1));
    return false;
  }

  // Give 'em the juice!
  while (target->level() < level) target->gainLevel();

  target->send("%s has {Gpromoted{x you to level {G%d{x!", target->seeName(avatar(), true).c_str(), target->level());
  avatar()->send("%s has been {Gpromoted{x to level {G%d{x!", target->identifiers().shortname().c_str(), target->level());

  return true;
}

CmdPurge::CmdPurge(void) {
  name("purge");
  addSyntax(0, "");
  level(Being::GOD);
  brief("Destroys all Items lying in the room.");
  return;
}

bool CmdPurge::execute(Being* being, const std::vector<std::string>& args) {
  Room* r = avatar()->room();
  Mob* mob = NULL;
  std::string ignored;
  // destroy beings...
  for (std::list<Being*>::iterator it = r->beings().begin(); it != r->beings().end();) {
    if ((*it)->isMob()) {
      mob = (Mob*)*it;
      // check to make sure the mob isn't on anything...
      if (!mob->isStanding()) {
        if (!mob->stand(ignored)) {
          avatar()->send("You couldn't force %s to stand up.", mob->identifiers().shortname().c_str());
          return false;
        }
      }
      delete *it;
      it = r->beings().erase(it);
    } else {
      ++it;
    }
  }
  // destroy items...
  r->inventory().purgeItems();
  // some flair...
  being->send("You've purged the room.");
  being->room()->send_cond("$p has purged the room.", being);
  return true;
}

CmdPut::CmdPut(void) {
  name("put");
  addSyntax(2, "<item> <container>");
  brief("Places an item into a container.");
  return;
}

bool CmdPut::execute(Being* being, const std::vector<std::string>& args) {
  std::list<Item*> items;
  Item* container = NULL;

  if ((container = being->findItem(args[1])) == NULL) {
    being->send("You can't find that.");
    return false;
  }
  if (!container->isContainer()) {
    being->send("That's not a container.");
    return false;
  }

  items = being->inventory().searchItems(args[0]);
  for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
    if ((*it) == container) {
      // we don't want anyone to be able to fold a knapsack up inside itself ;-)
      continue;
    }
    if ((*it)->flags().test(ITEM_NODROP)) {
      // make sure they can let go of it
      being->send("You can't let go of %s{x.\n", (*it)->identifiers().shortname().c_str());
    } else if ((*it)->isContainer()) {
      // until MySQL gets das boot, this is too much hassle
      being->send("You can't put a container inside another container.\n");
    } else {
      // transfer the item
      being->inventory().remove(*it);
      container->container()->inventory().add(*it);
      being->send("You put %s{x in %s{x.\n", (*it)->identifiers().shortname().c_str(), container->identifiers().shortname().c_str());
      being->room()->send_cond("$p puts $o{x in $O{x.\n", being, *it, container);
      return true;
    }
  }

  return false;
}
