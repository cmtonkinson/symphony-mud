
#include "avatar.h"
#include "command-def.h"
#include "display.h"
#include "io-handler.h"
#include "item-types.h"
#include "room.h"
#include "world.h"
#include "zone.h"

CmdNedit::CmdNedit(void) {
  name("nedit");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  addSyntax(2, "create <vnum>");
  brief("Launches the Npc Editor.");
  return;
}

bool CmdNedit::execute(Being* being, const std::vector<std::string>& args) {
  std::map<unsigned long,Npc*>::iterator it;
  Zone* zone = NULL;
  Npc* npc = NULL;
  unsigned long vnum = 0;

  if (args.size() == 1) {
    vnum = estring(args[0]);
    // Get the zone...
    if ((zone = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!zone->hasPermission(avatar())) {
      avatar()->send("You don't have permissions to that item.");
      return false;
    }
    // Make sure the Item exists...
    if ((it = zone->npcs().find(vnum)) == zone->npcs().end()) {
      avatar()->send("That npc doesn't exist.");
      return false;
    }
    // Make sure no one else is editing the item...
    for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
      if (a_it->second->mode().number() == MODE_NEDIT && a_it->second->nedit() == it->second) {
        avatar()->send("Sorry, %s is currently editing %s (npc %lu).", avatar()->seeName(((Being*)a_it->second)).c_str(), it->second->identifiers().shortname().c_str(), it->second->vnum());
        return false;
      }
    }
    // All looks well; send them to nedit...
    avatar()->nedit(it->second);
    avatar()->pushIOHandler(new NeditIOHandler(avatar()));
    avatar()->send("You're editing npc %lu.", avatar()->nedit()->vnum());
    return true;
  } else if (args.size() == 2) {
    if (!Regex::strPrefix(args[0], "create")) {
      avatar()->send(printSyntax());
      return false;
    }
    vnum = estring(args[1]);
    // Get the zone...
    if ((zone = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!zone->hasPermission(avatar())) {
      avatar()->send("You don't have permissions to that npc.");
      return false;
    }
    // Make sure the Npc doesn't already exist...
    if ((it = zone->npcs().find(vnum)) != zone->npcs().end()) {
      avatar()->send("That npc already exists.");
      return false;
    }
    // Everything checks out; let's make us a new Npc!
    if ((npc = Npc::create(zone, vnum)) == NULL) {
      avatar()->send("There was an error creating the npc.");
      return false;
    }
    avatar()->send("Npc %lu created successfully.", npc->vnum());
    avatar()->mode().set(MODE_NEDIT);
    avatar()->nedit(npc);
    avatar()->pushIOHandler(new NeditIOHandler(avatar()));
    return true;
  }

  return false;
}

CmdNlist::CmdNlist(void) {
  name("nlist");
  level(Being::DEMIGOD);
  addSyntax(1, "<zoneID>                       (list all Npcs in the zone)");
  addSyntax(2, "<first vnum> <last vnum>       (list all Npcs in the vnum range)");
  addSyntax(1, "<keyword>                      (list all Npcs by keyword)");
  addSyntax(1, "/<regex>                       (list all Npcs matching the PCRE)");
  return;
}

bool CmdNlist::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args(args);
  std::vector<Npc*> npcs;
  Zone* zone = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  if (mutable_args.size() == 1) {
    if (Regex::match("^[0-9]+$", mutable_args[0])) {
      // We got an zoneID...
      if ((zone = World::Instance().findZone(estring(mutable_args[0]))) == NULL) {
        being->send("That zone couldn't be found.");
        return false;
      }
      for (std::map<unsigned long,Npc*>::iterator m_it = zone->npcs().begin(); m_it != zone->npcs().end(); ++m_it) {
        npcs.push_back(m_it->second);
      }
    } else {
      if (mutable_args[0][0] == '/') {
        mutable_args[0].erase(0, 1);
        // This search is a regex...
        for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
          for (std::map<unsigned long,Npc*>::iterator m_it = (*a_it)->npcs().begin(); m_it != (*a_it)->npcs().end(); ++m_it) {
            if (m_it->second->identifiers().matchesKeyword(mutable_args[0])) {
              npcs.push_back(m_it->second);
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
          for (std::map<unsigned long,Npc*>::iterator m_it = (*a_it)->npcs().begin(); m_it != (*a_it)->npcs().end(); ++m_it) {
            if (m_it->second->identifiers().matchesKeyword(search)) {
              npcs.push_back(m_it->second);
            }
          }
        }
      }
    }
  } else if (mutable_args.size() == 2) {
    /* We're looking for a vnum range here */
    // Grab our range values...
    low = estring(mutable_args[0]);
    high = estring(mutable_args[1]);
    // Check our range...
    if (!high || low >= high) {
      being->send("Invalid vnum range.");
      return false;
    }
    if (low+400 < high) {
      being->send("The maximum vnum range is 400.");
      return false;
    }
    // Grab the npcs...
    for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
      for (std::map<unsigned long,Npc*>::iterator m_it = (*a_it)->npcs().begin(); m_it != (*a_it)->npcs().end(); ++m_it) {
        if (m_it->second->vnum() >= low && m_it->second->vnum() <= high) {
          npcs.push_back(m_it->second);
        }
      }
    }
  }

  if (npcs.empty()) {
    being->send("No matches for \"%s\"", mutable_args[0].c_str());
    return false;
  }

  output.append(" [{y vnum{x] {gname{x\n -------------------\n");
  for (std::vector<Npc*>::iterator it = npcs.begin(); it != npcs.end(); ++it) {
    sprintf(buffer, " [{y%5u{x] %s{x\n", (*it)->vnum(), (*it)->identifiers().shortname().c_str());
    output.append(buffer);
  }

  being->send(output);
  return true;
}

CmdNload::CmdNload(void) {
  name("nload");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  brief("Incarnates a Npc.");
}

bool CmdNload::execute(Being* being, const std::vector<std::string>& args) {
  unsigned long vnum = estring(args[0]);
  std::map<unsigned long,Npc*>::iterator m_it;
  Npc* npc = NULL;

  for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
    if ((m_it = (*a_it)->npcs().find(vnum)) != (*a_it)->npcs().end()) {
      npc = Npc::create(m_it->second, being->room());
      if (npc == NULL) {
        being->send("Nload failed. Don't you feel stupid?");
        return false;
      }
      if (npc->identifiers().shortname().empty() || npc->identifiers().longname().empty() || npc->identifiers().getKeywords().empty()) {
        avatar()->send("Sorry; that npc isn't complete yet.");
        return false;
      }
      World::Instance().insert(npc);
      being->room()->add(npc);
      npc->room(being->room());
      npc->npcilize();
      being->send("You load %s.", npc->identifiers().shortname().c_str());
      being->room()->send_cond("$p has created $c.", being, npc);
      return true;
    }
  }

  being->send("There is no npc of that vnum.");
  return false;
}

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
  addSyntax(2, "<npc> <level>");
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
  Npc* npc = NULL;
  std::string ignored;
  // destroy beings...
  for (std::list<Being*>::iterator it = r->beings().begin(); it != r->beings().end();) {
    if ((*it)->isNpc()) {
      npc = (Npc*)*it;
      // check to make sure the npc isn't on anything...
      if (!npc->isStanding()) {
        if (!npc->stand(ignored)) {
          avatar()->send("You couldn't force %s to stand up.", npc->identifiers().shortname().c_str());
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
