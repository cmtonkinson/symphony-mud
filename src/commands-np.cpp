/*
 * $Id: commands-np.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "commandTable.h"
#include "commandTable-default.h"
#include "display.h"
#include "io-handler.h"
#include "object-container.h"
#include "room.h"

#include "world.h"

/*
Cmd::Cmd(void) {
  name("");
  level();
  addSyntax(0, "");
  brief("");
  return;
}

bool Cmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

CmdNote::CmdNote(void) {
  name("note");
  playerOnly(true);
  addSyntax(1, "write");
  addSyntax(2, "read <number>");
  brief("Manipulates notes in the board system.");
  return;
}

bool CmdNote::execute(Creature* creature, const std::vector<std::string>& args) {
  std::map<unsigned,Board*>::iterator b_it;
  std::map<unsigned long,Note*>::iterator n_it;
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
      avatar()->note(new Note(board->number(), avatar()->ID()));
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
        avatar()->send("Note: {c%lu{x\n", note->ID());
        avatar()->send("Author: {y%s{x\n", World::Instance().getAvatarNameByID(note->author()).c_str());
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

CmdOedit::CmdOedit(void) {
  name("oedit");
  level(DEMIGOD);
  addSyntax(1, "<vnum>");
  addSyntax(2, "create <vnum>");
  brief("Invokes the Object Editor.");
  return;
}

bool CmdOedit::execute(Creature* creature, const std::vector<std::string>& args) {
  std::map<unsigned long,Object*>::iterator it;
  Area* area = NULL;
  Object* object = NULL;
  unsigned long vnum = 0;

  if (args.size() == 1) {
    vnum = estring(args[0]);
    // Get the area...
    if ((area = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!World::Instance().hasPermission(area, avatar())) {
      avatar()->send("You don't have permissions to that object.");
      return false;
    }
    // Make sure the Object exists...
    if ((it = area->objects().find(vnum)) == area->objects().end()) {
      avatar()->send("That object doesn't exist.");
      return false;
    }
    // Make sure no one else is editing the object...
    for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
      if (a_it->second->mode().number() == MODE_OEDIT && a_it->second->oedit() == it->second) {
        avatar()->send("Sorry, %s is currently editing %s (object %lu).", avatar()->seeName(((Creature*)a_it->second)).c_str(), it->second->identifiers().shortname().c_str(), it->second->vnum());
        return false;
      }
    }
    // All looks well; send them to oedit...
    avatar()->oedit(it->second);
    avatar()->pushIOHandler(new OeditIOHandler(avatar()));
    avatar()->send("You're editing object %lu.", avatar()->oedit()->vnum());
    return true;
  }

  if (args.size() == 2 && Regex::strPrefix(args[0], "create")) {
    vnum = estring(args[1]);
    // Get the area...
    if ((area = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!World::Instance().hasPermission(area, (Avatar*)creature)) {
      avatar()->send("You don't have permissions to that object.");
      return false;
    }
    // Make sure the Object doesn't already exist...
    if ((it = area->objects().find(vnum)) != area->objects().end()) {
      avatar()->send("That object already exists.");
      return false;
    }
    // Everything checks out; let's make us a new Object!
    object = new Object();
    object->vnum(vnum);
    area->objects()[object->vnum()] = object;
    try {
      object->insert(World::Instance().getMysql(), area->ID());
    } catch (MysqlException me) {
      avatar()->send("Failed to create object in database.");
      return false;
    }
    avatar()->send("Object %lu created successfully.", object->vnum());
    avatar()->mode().set(MODE_OEDIT);
    avatar()->oedit(object);
    avatar()->pushIOHandler(new OeditIOHandler(creature));
    return true;
  }

  avatar()->send(printSyntax());
  return false;
}

CmdOlist::CmdOlist(void) {
  name("olist");
  level(DEMIGOD);
  addSyntax(1, "<areaID>                       (list all Objects in the area)");
  addSyntax(2, "<first vnum> <last vnum>       (list all Objects in the vnum range)");
  addSyntax(1, "<keyword>                      (list all Objects by keyword)");
  addSyntax(1, "/<regex>                       (list all Objects matching the PCRE)");
  return;
}

bool CmdOlist::execute(Creature* creature, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args = args;
  std::vector<Object*> objects;
  Area* area = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[MAX_BUFFER];

  if (mutable_args.size() == 1) {
    if (Regex::match("^[0-9]+$", mutable_args[0])) {
      // We got an areaID...
      if ((area = World::Instance().findArea(estring(mutable_args[0]))) == NULL) {
        creature->send("That area couldn't be found.");
        return false;
      }
      for (std::map<unsigned long,Object*>::iterator o_it = area->objects().begin(); o_it != area->objects().end(); ++o_it) {
        objects.push_back(o_it->second);
      }
    } else {
      if (mutable_args[0][0] == '/') {
        mutable_args[0].erase(0, 1);
        // This search is a regex...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Object*>::iterator o_it = (*a_it)->objects().begin(); o_it != (*a_it)->objects().end(); ++o_it) {
            if (o_it->second->identifiers().matchesKeyword(mutable_args[0])) {
              objects.push_back(o_it->second);
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Object*>::iterator o_it = (*a_it)->objects().begin(); o_it != (*a_it)->objects().end(); ++o_it) {
            if (o_it->second->identifiers().matchesKeyword(search)) {
              objects.push_back(o_it->second);
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
      creature->send("Invalid vnum range.");
      return false;
    }
    if (low+400 < high) {
      creature->send("The maximum vnum range is 400.");
      return false;
    }
    // Grab the rooms...
    for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
      for (std::map<unsigned long,Object*>::iterator o_it = (*a_it)->objects().begin(); o_it != (*a_it)->objects().end(); ++o_it) {
        if (o_it->second->vnum() >= low && o_it->second->vnum() <= high) {
          objects.push_back(o_it->second);
        }
      }
    }
  } else {
    creature->send(printSyntax());
    return false;
  }

  if (objects.empty()) {
    creature->send("No matches for \"%s\"", mutable_args[0].c_str());
    return false;
  }

  output.append(" [{y vnum{x] {gname{x\n -------------------\n");
  for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
    sprintf(buffer, " [{y%5lu{x] %s{x\n", (*it)->vnum(), (*it)->identifiers().shortname().c_str());
    output.append(buffer);
  }

  creature->send(output);
  return true;
}

CmdOload::CmdOload(void) {
  name("oload");
  level(DEMIGOD);
  addSyntax(1, "<vnum>");
  brief("Fabricates an Object.");
}

bool CmdOload::execute(Creature* creature, const std::vector<std::string>& args) {
  unsigned long vnum = estring(args[0]);
  std::map<unsigned long,Object*>::iterator o_it;
  Object* object = NULL;

  for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
    if ((o_it = (*a_it)->objects().find(vnum)) != (*a_it)->objects().end()) {
      object = new Object(*o_it->second);
      if (object == NULL) {
        creature->send("Oload failed.");
        return false;
      }
      if (object->identifiers().shortname().empty() || object->identifiers().longname().empty() || object->identifiers().getKeywords().empty()) {
        avatar()->send("Sorry; that object isn't complete yet.");
        return false;
      }
      World::Instance().insert(object);
      creature->inventory().add(object);
      creature->send("You load %s.", object->identifiers().shortname().c_str());
      creature->room()->send_cond("$p has created $o.", creature, object);
      return true;
    }
  }

  creature->send("There is no object of that vnum.");
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

bool CmdOoc::execute(Creature* creature, const std::vector<std::string>& args) {
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

bool CmdOpen::execute(Creature* creature, const std::vector<std::string>& args) {
  Exit* exit = NULL;
  Exit* inv = NULL;

  if ((exit = creature->room()->exit(args[0])) == NULL) {
    creature->send("There is no door in that direction.");
    return false;
  }

  if (!exit->flag(EXIT_CLOSED)) {
    creature->send("It's already open.");
    return false;
  }

  if (exit->flag(EXIT_LOCKED)) {
    creature->send("It's locked.");
    return false;
  }

  exit->flag(EXIT_CLOSED, false);
  creature->send("You open the door.");
  creature->room()->send("$p opened the way $e.", creature, exit);
  if ((inv = exit->target()->exit(Exit::inverse(exit->direction().number()))) != NULL) {
    exit->target()->send("The way $e has been opened.", NULL, inv);
  }

  return true;
}

CmdPassword::CmdPassword(void) {
  name("password");
  addSyntax(3, "<old pw> <new pw> <confirm new pw>");
  brief("Changes your account password.");
  return;
}

bool CmdPassword::execute(Creature* creature, const std::vector<std::string>& args) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    char query[MAX_BUFFER];

    if (args[1].compare(args[2]) != 0) {
      creature->send("The password you entered does not match the confirmation.");
      return false;
    }

    sprintf(query,
      "UPDATE avatars                 \
       SET password = PASSWORD('%s')  \
       WHERE avatarID = %lu           \
       AND password = PASSWORD('%s')  \
       LIMIT 1;",
      args[1].c_str(),
      creature->ID(),
      args[0].c_str()
   );
    if (mysql->update(query) == 1) {
      creature->send("Password successfully updated.");
      return true;
    } else {
      creature->send("Password update failed.");
      return false;
    }

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to save avatar %lu: %s\n", creature->ID(), me.getMessage().c_str());
    World::Instance().playerLog(ERROR, LOG_AVATAR, "failed password reset for %s (%lu)", creature->identifiers().shortname().c_str(), creature->ID());
    creature->send("Command failed. This issue has been logged.");
    return false;
  }

}

CmdPeace::CmdPeace(void) {
  name("peace");
  playerOnly(true);
  level(GOD);
  addSyntax(0, "");
  addSyntax(1, "all");
  brief("Ends all combat, in the current room, or the whole world.");
  return;
}

bool CmdPeace::execute(Creature* creature, const std::vector<std::string>& args) {
  if (args[0].empty()) {
    for (std::list<Creature*>::iterator iter = avatar()->room()->creatures().begin(); iter != avatar()->room()->creatures().end(); ++iter) {
      (*iter)->peace();
    }
    avatar()->room()->send_cond("$p has imposed peace here.\n", creature);
    avatar()->send("You have imposed peace here.\n");
  } else {
    for (std::set<Creature*>::iterator iter = World::Instance().getCreatures().begin(); iter != World::Instance().getCreatures().end(); ++iter) {
      (*iter)->peace();
      if (*iter == creature) continue;
      (*iter)->send((*iter)->seeName(avatar(), true));
      (*iter)->send(" has imposed peace throughout the realm.\n");
    }
    avatar()->send("You have imposed peace throughout the realm.\n");
  }
  return true;
}

CmdPedit::CmdPedit(void) {
  name("pedit");
  level(GOD);
  addSyntax(1, "<player>       (must be connected)");
  brief("Invokes the Player Editor.");
  return;
}

bool CmdPedit::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  // Find the player...
  if ((target = World::Instance().findAvatar(args[0])) == NULL) {
    avatar()->send("That player isn't around.");
    return false;
  }

  // Make sure noone else is in Pedit mode for the player...
  // Make sure no one else is editing the object...
  for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
    if (a_it->second->mode().number() == MODE_PEDIT && a_it->second->pedit() == target) {
      avatar()->send("Sorry, %s is currently editing %s.", avatar()->seeName(((Creature*)a_it->second)).c_str(), target->identifiers().shortname().c_str());
      return false;
    }
  }

  if (avatar()->canAlter(target) || avatar()->level() >= CREATOR) {
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
  level(DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your Poof-In.");
  return;
}

bool CmdPoofin::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)creature;

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
  level(DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your Poof-Out.");
  return;
}

bool CmdPoofout::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)creature;

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
  level(CREATOR);
  brief("Advances the target to the given level.");
  return;
}

bool CmdPromote::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* target = NULL;
  unsigned level = estring(args[1]);

  // do the checks...
  if ((target = avatar()->findCreature(args[0])) == NULL) {
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
  if (level >= (unsigned)avatar()->level() - ALTERABILITY_LEVEL_DIFFERENCE) {
    avatar()->send("You can't promote anyone higher than level %d.", avatar()->level() - (ALTERABILITY_LEVEL_DIFFERENCE-1));
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
  level(GOD);
  brief("Destroys all Objects lying in the room.");
  return;
}

bool CmdPurge::execute(Creature* creature, const std::vector<std::string>& args) {
  Room* r = avatar()->room();
  Mob* mob = NULL;
  std::string ignored;
  // destroy creatures...
  for (std::list<Creature*>::iterator it = r->creatures().begin(); it != r->creatures().end();) {
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
      it = r->creatures().erase(it);
    } else {
      ++it;
    }
  }
  // destroy objects...
  r->inventory().purgeObjects();
  // some flair...
  creature->send("You've purged the room.");
  creature->room()->send_cond("$p has purged the room.", creature);
  return true;
}

CmdPut::CmdPut(void) {
  name("put");
  addSyntax(2, "<object> <container>");
  brief("Places an object into a container.");
  return;
}

bool CmdPut::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> objects;
  Object* container = NULL;

  if ((container = creature->findObject(args[1])) == NULL) {
    creature->send("You can't find that.");
    return false;
  }
  if (!container->isContainer()) {
    creature->send("That's not a container.");
    return false;
  }

  objects = creature->inventory().searchObjects(args[0]);
  for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
    if ((*it) == container) {
      // we don't want anyone to be able to fold a knapsack up inside itself ;-)
      continue;
    }
    if ((*it)->flags().test(OBJECT_NODROP)) {
      // make sure they can let go of it
      creature->send("You can't let go of %s{x.\n", (*it)->identifiers().shortname().c_str());
    } else {
      // transfer the object
      creature->inventory().remove(*it);
      container->container()->inventory().add(*it);
      creature->send("You put %s{x in %s{x.\n", (*it)->identifiers().shortname().c_str(), container->identifiers().shortname().c_str());
      creature->room()->send_cond("$p puts $o{x in $O{x.\n", creature, *it, container);
    }
  }

  return false;
}
