/*
 * $Id: commands-af.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include <algorithm>
#include "area.h"
#include "avatar.h"
#include "commandTable.h"
#include "commandTable-default.h"
#include "display.h"
#include "flagTable.h"
#include "io-handler.h"
#include "room.h"

#include "world.h"

/*
Cmd::Cmd(void) {
  name("");
  playerOnly();
  level();
  allowedWhile();
  addSyntax(0, "");
  brief("");
  return;
}

bool Cmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

CmdAdminNote::CmdAdminNote(void) {
  name("adminnote");
  level(CREATOR);
  addSyntax(-1, "<string>");
  brief("Broadcasts a message to all connected players.");
  return;
}

bool CmdAdminNote::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string message;
  message.assign("{YAdministrative Note{x: {y").append(args[0]).append("{x\n");
  World::Instance().broadcast(message);
  return true;
}

CmdAedit::CmdAedit(void) {
  name("aedit");
  level(GOD);
  addSyntax(1, "<areaID>");
  addSyntax(3, "create <first vnum> <area size>");
  brief("Launches the Area Editor.");
  return;
}

bool CmdAedit::execute(Creature* creature, const std::vector<std::string>& args) {
  std::set<Area*,area_comp>::iterator it;
  Area* area = NULL;
  unsigned short minimum = 25;
  unsigned short multiple = 25;

  if (args.size() == 3 && args[0] == "create") {
    unsigned long low = estring(args[1]);
    unsigned long size = estring(args[2]);
    unsigned long high = low + size - 1;
    // Check permissions...
    if (!(avatar()->adminFlags().test(ADMIN_HEADBUILDER) || avatar()->level() >= CREATOR)) {
      avatar()->send("Only the Head Builder can create new areas.");
      return false;
    }
    // Check to make sure vnums are good...
    if (low % minimum) {
      avatar()->send("The first vnum of an area must be a multiple of %lu.", multiple);
      return false;
    }
    if (size < minimum || size % minimum) {
      avatar()->send("Area sizes must be a multiple of %lu, and at least %lu.", multiple, minimum);
      return false;
    }
    for (it = World::Instance().getAreas().begin(); it != World::Instance().getAreas().end(); ++it) {
      if (((*it)->low() < low && low < (*it)->high()) || ((*it)->low() < high && high < (*it)->high())) {
        avatar()->send("That would cause a vnum collision with %s (area %lu).  Please check your numbers.", (*it)->name().c_str(), (*it)->ID());
        return false;
      }
    }
    area = new Area(low, high);
    if (!area->ID()) {
      avatar()->send("Something went wrong while creating the area.");
      World::Instance().worldLog(ERROR, LOG_WORLD, "%lu failed to create area from %lu through %lu.", avatar()->ID(), area->low(), area->high());
      delete area;
      return false;
    }
    avatar()->aedit(area);
    World::Instance().insert(area);
    avatar()->pushIOHandler(new AeditIOHandler(avatar()));
    avatar()->send("You've created a new area (number %lu) with vnums %lu through %lu.", area->ID(), area->low(), area->high());
  } else if (args.size() == 1) {
    // Check for the area...
    if ((area = World::Instance().findArea(estring(args[0]))) == NULL) {
      avatar()->send("That area doesn't exist.");
      return false;
    }
    // Check permissions...
    if ((area->ID() == 1 && avatar()->level() < CREATOR) || !World::Instance().hasPermission(area, avatar())) {
      avatar()->send("You can't edit %s.", area->name().c_str());
      return false;
    }
    // Make sure no one else is editing the area...
    for (std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
      if (it->second->mode().number() == MODE_AEDIT && it->second->aedit() == area) {
        avatar()->send("Sorry, %s is currently editing %s (area %lu).", avatar()->seeName(((Creature*)it->second)).c_str(), area->name().c_str(), area->ID());
        return false;
      }
    }
    // Send them on to the editor...
    avatar()->send("You're editing %s (%lu).", area->name().c_str(), area->ID());
    avatar()->mode().set(MODE_AEDIT);
    avatar()->aedit(area);
    avatar()->pushIOHandler(new AeditIOHandler(avatar()));
  }

  return true;
}

CmdAfk::CmdAfk(void) {
  name("afk");
  playerOnly(true);
  addSyntax(0, "");
  brief("Marks you as \"Away From Keyboard.\"");
  return;
}

bool CmdAfk::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string message;

  if (avatar()->whoFlags().test(WHO_AFK)) {
    avatar()->whoFlags().clear(WHO_AFK);
    message = "Welcome back!";
  } else {
    avatar()->whoFlags().set(WHO_AFK);
    message = "You're now marked as \"away from keyboard.\"";
  }

  avatar()->send(message);
  return true;
}

CmdAreas::CmdAreas(void) {
  name("areas");
  playerOnly(true);
  addSyntax(0, "");
  brief("List the Areas of the World.");
  return;
}

bool CmdAreas::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output("Areas:");
  char buffer[MAX_BUFFER];

  for (std::set<Area*,area_comp>::iterator it = World::Instance().getAreas().begin(); it != World::Instance().getAreas().end(); ++it) {
    if (avatar()->level() >= DEMIGOD) {
      if (World::Instance().hasPermission(*it, avatar())) {
        sprintf(buffer, "\n ({Y%3lu{x) [ {C%4lu{x - {C%4lu{x ] {M%s{x", (*it)->ID(), (*it)->low(), (*it)->high(), (*it)->name().c_str());
      } else {
        sprintf(buffer, "\n ({y%3lu{x) [ {c%4lu{x - {c%4lu{x ] {m%s{x", (*it)->ID(), (*it)->low(), (*it)->high(), (*it)->name().c_str());
      }
    } else {
      sprintf(buffer, "\n %s", (*it)->name().c_str());
    }
    output.append(buffer);
  }

  avatar()->send(output);
  return true;
}

CmdAt::CmdAt(void) {
  name("at");
  playerOnly(true);
  level(DEMIGOD);
  addSyntax(-2, "<player> <command>");
  addSyntax(-2, "<mob> <command>");
  addSyntax(-2, "<vnum> <command>");
  brief("Executes the given command as if you were in the same room as the target.");
  return;
}

bool CmdAt::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* foo = NULL;
  Room* current = avatar()->room();
  Room* target = NULL;
  bool success = false;

  if (Regex::match("^[0-9]+$", args[0])) {
    if ((target = World::Instance().findRoom(estring(args[0]))) == NULL) {
      avatar()->send("That room doesn't exist.");
      return false;
    }
  } else {
    if ((foo = World::Instance().findCreature(args[0])) == NULL) {
      avatar()->send("They aren't around at the moment.");
      return false;
    }
    target = foo->room();
  }

  avatar()->room()->remove(avatar());
  avatar()->room(target);
  target->add(avatar());
  success = avatar()->IOhandler()->handle(args[1]);
  target->remove(avatar());
  avatar()->room(current);
  current->add(avatar());

  return success;
}

CmdBigBrother::CmdBigBrother(void) {
  name("bigbrother");
  level(DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "on");
  addSyntax(1, "off");
  addSyntax(2, "<subchannel> on");
  addSyntax(2, "<subchannel> off");
  brief("Toggles what you see on the BigBrother administrative information channel.");
  return;
}

bool CmdBigBrother::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output;
  std::string on = "[{GON{x]";
  std::string off = "[{rOFF{x]";
  bool toggle = false;
  bool status = true;

  // No arguments...
  if (args[0] == "") {
    output.append("\nBigBrother:                   ").append(avatar()->adminFlags().test(ADMIN_BIGBROTHER) ? on : off).append("\n");
    output.append("\n -Logins/Logouts  (\"logins\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_LOGINS) ? on : off);
    output.append("\n -Edit modes      (\"modes\")   ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_MODES) ? on : off);
    output.append("\n -Player deaths   (\"deaths\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_DEATHS) ? on : off);
    output.append("\n -World Schedule  (\"events\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_EVENTS) ? on : off);
    output.append("\n -System Events   (\"system\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_SYSTEM) ? on : off);
    output.append("\n -World Changes   (\"changes\") ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_CHANGES) ? on : off);
    output.append("\n -Server Errors   (\"errors\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_ERRORS) ? on : off);
    output.append("\n -Area Resets     (\"resets\")  ").append(avatar()->adminFlags().test(ADMIN_BIGBRO_RESETS) ? on : off);
  } else if (args.size() == 1) {
    output.append("BigBrother is now ");
    if (args[0] == "on") {
      avatar()->adminFlags().set(ADMIN_BIGBROTHER);
      output.append(on);
    } else {
      avatar()->adminFlags().clear(ADMIN_BIGBROTHER);
      output.append(off);
    }
  } else if (args.size() == 2) {
    toggle = args[1] == "on" ? true : false;
    if (toggle) {
      avatar()->adminFlags().set(ADMIN_BIGBROTHER);
    }
    if (args[0] == "logins") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_LOGINS, toggle);
      output.append("BigBrother login module is now ").append(toggle ? on : off);
    } else if (args[0] == "modes") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_MODES, toggle);
      output.append("BigBrother modes module is now ").append(toggle ? on : off);
    } else if (args[0] == "deaths") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_DEATHS, toggle);
      output.append("BigBrother deaths module is now ").append(toggle ? on : off);
    } else if (args[0] == "events") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_EVENTS, toggle);
      output.append("BigBrother events module is now ").append(toggle ? on : off);
    } else if (args[0] == "system") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_SYSTEM, toggle);
      output.append("BigBrother system module is now ").append(toggle ? on : off);
    } else if (args[0] == "changes") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_CHANGES, toggle);
      output.append("BigBrother changes module is now ").append(toggle ? on : off);
    } else if (args[0] == "errors") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_ERRORS, toggle);
      output.append("BigBrother errors module is now ").append(toggle ? on : off);
    } else if (args[0] == "resets") {
      avatar()->adminFlags().set(ADMIN_BIGBRO_RESETS, toggle);
      output.append("BigBrother resets module is now ").append(toggle ? on : off);
    }/* else if (args[0] == "all") {
      avatar()->adminFlags().set(SET_ALL, toggle);
      output.append("All BigBrother subsystems are now ").append(toggle ? on : off);
    }*/ else {
      output.append("Module name not recognized");
      status = false;
    }
  }

  avatar()->send(output);
  return status;
}

CmdBored::CmdBored(void) {
  name("bored");
  playerOnly(true);
  addSyntax(0, "");
  brief("Marks you as being bored.");
  return;
}

bool CmdBored::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string message;

  if (avatar()->whoFlags().test(WHO_BORED)) {
    avatar()->whoFlags().clear(WHO_BORED);
    message = "You're no longer marked as \"bored.\"";
  } else {
    avatar()->whoFlags().set(WHO_BORED);
    message = "You're now marked as \"bored.\"";
  }

  avatar()->send(message);
  return true;
}

CmdBoards::CmdBoards(void) {
  name("boards");
  playerOnly(true);
  addSyntax(0, "             (list the available message boards)");
  addSyntax(1, "list         (list unread messages from the selected board)");
  addSyntax(1, "<number>     (show messages from the selected board)");
  brief("Peruse the message boards.");
  return;
}

bool CmdBoards::execute(Creature* creature, const std::vector<std::string>& args) {
  std::map<unsigned,Board*>::iterator b_it;
  std::map<unsigned long,Note*>::iterator n_it;
  unsigned board = 0;

  if (args[0] == "") {
    avatar()->send(" {CNote:{x This is {Rnot{x the venue for bug reports.  They belong either in Google\n Code (for core Symphony components) or else in BaseCamp (for our specific\n game features).\n\n");
    avatar()->send("  +-------+----------------------+-------------------+\n");
    avatar()->send("  | {cBoard{x | {CName{x                 | {BUnread{x/{bTotal{x      |\n");
    avatar()->send("  +-------+----------------------+-------------------+\n");
    for (b_it = World::Instance().boards().begin(); b_it != World::Instance().boards().end(); ++b_it) {
      avatar()->send("%s| {c%2u{x    | {C%-20s{x |   {B%4u{x/{b%-4u{x       |\n",
        b_it->first == avatar()->board() ? "->" : "  ",
        b_it->second->number(),
        b_it->second->name(),
        b_it->second->notes().size(),
        b_it->second->notes().size()
     );
    }
    avatar()->send("  +-------+----------------------+-------------------+\n");
  } else if (args.size() == 1) {
    if (args[0] == "list") {
      board = avatar()->board();
    } else {
      board = estring(args[0]);
    }
    if ((b_it = World::Instance().boards().find(board)) == World::Instance().boards().end()) {
      avatar()->send("That board wasn't found.");
      return false;
    } else {
      avatar()->board(b_it->first);
      avatar()->send("      Notes on the {C%s{x board:\n", b_it->second->name());
      avatar()->send(" +------+--------------+----------------------------------------------------+\n");
      avatar()->send(" | {cNote{x | {yAuthor{x       | {YSubject{x                                            |\n");
      avatar()->send(" +------+--------------+----------------------------------------------------+\n");
      for (n_it = b_it->second->notes().begin(); n_it != b_it->second->notes().end(); ++n_it) {
        avatar()->send(" | {c%4lu{x | {y%-12s{x | {Y%-50s{x |\n", n_it->second->ID(), World::Instance().getAvatarNameByID(n_it->second->author()).c_str(), n_it->second->subject().c_str());
      }
      avatar()->send(" +------+--------------+----------------------------------------------------+\n");
    }
  }

  return true;
}

CmdBusy::CmdBusy(void) {
  name("busy");
  playerOnly(true);
  addSyntax(0, "");
  brief("Marks you as being \"busy.\"");
  return;
}

bool CmdBusy::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string message;

  if (avatar()->whoFlags().test(WHO_BUSY)) {
    avatar()->whoFlags().clear(WHO_BUSY);
    message = "You're no longer marked as \"busy.\"";
  } else {
    avatar()->whoFlags().set(WHO_BUSY);
    message = "You're now marked as \"busy.\"";
  }

  avatar()->send(message);
  return true;
}

CmdCast::CmdCast(void) {
  name("cast");
  shortcut("c");
  allowedWhile(STANDING);
  addSyntax(1, "<spell>");
  seeAlso("spells");
  brief("Use mana to invoke a magical spell.");
  return;
}

bool CmdCast::execute(Creature* creature, const std::vector<std::string>& args) {
  Ability* spell = NULL;

  if ((spell = creature->find_spell(args[0])) == NULL) {
    creature->send("What spell was that again?\n");
    return false;
  }

  return creature->intone(spell);
}

CmdChannels::CmdChannels(void) {
  name("channels");
  playerOnly(true);
  addSyntax(0, "");
  brief("Displays which communication channels you see, and which you don't.");
  return;
}

bool CmdChannels::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output;
  std::string on = "{x[{GON{x]";
  std::string off = "{x[{rOFF{x]";
  output.append("\nCurrent channels:");
  output.append("\n -Tell/Reply:     ").append(avatar()->channelFlags().test(CHANNEL_TELL) ? on : off);
  output.append("\n -OOC:            ").append(avatar()->channelFlags().test(CHANNEL_OOC) ? on : off);
  avatar()->send(output);
  return true;
}

CmdClone::CmdClone(void) {
  name("clone");
  level(DEMIGOD);
  addSyntax(2, "<object> <times>");
  brief("Make an exact copy of an existing object.");
  return;
}

bool CmdClone::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* object = NULL;
  Object* clone = NULL;
  unsigned times = 0;
  char buf[10];

  if ((object = avatar()->inventory().searchSingleObject(args[0])) == NULL) {
    avatar()->send("You aren't carrying that.");
    return false;
  }

  times = estring(args[1]);
  sprintf(buf, "%d", times);

  if (times == 0) {
    avatar()->send("What would be the point in making zero copies?");
    return false;
  } else if (times > 100) {
    avatar()->send("Sorry, only up to 100 at a time.");
    return false;
  }

  for (unsigned u = 0; u < times; ++u) {
    clone = new Object(*object);
    avatar()->inventory().add(clone);
  }

  if (times == 1) {
    avatar()->send("You've created a clone of %s.\n", clone->identifiers().shortname().c_str());
    avatar()->room()->send_cond("$p has created a clone of $o.\n", avatar(), object);
  } else {
    avatar()->send("You've created %d clones of %s.\n", times, clone->identifiers().shortname().c_str());
    avatar()->room()->send_cond("$p has created $s clones of $O.\n", avatar(), buf, object);
  }

  return true;
}

CmdClose::CmdClose(void) {
  name("close");
  addSyntax(1, "<direction>");
  brief("Closes a door.");
  return;
}

bool CmdClose::execute(Creature* creature, const std::vector<std::string>& args) {
  Exit* exit = NULL;
  Exit* inv = NULL;

  if ((exit = creature->room()->exit(args[0])) == NULL) {
    creature->send("There is no door in that direction.");
    return false;
  }
  if (exit->flag(EXIT_CLOSED)) {
    creature->send("It's already closed.");
    return false;
  }
  if (!exit->flag(EXIT_CLOSEABLE)) {
    creature->send("You can't close that.");
    return false;
  }

  exit->flag(EXIT_CLOSED, true);
  creature->send("You close the door.");
  creature->room()->send("$p closed the way $e.", creature, exit);
  if ((inv = exit->target()->exit(Exit::inverse(exit->direction().number()))) != NULL) {
    exit->target()->send("The way $e has been closed.", NULL, inv);
  }
  return true;
}

CmdCommands::CmdCommands(void) {
  name("commands");
  playerOnly(true);
  addSyntax(0, "");
  brief("Lists the available commands.");
  return;
}

bool CmdCommands::execute(Creature* creature, const std::vector<std::string>& args) {
  CommandTable* table = avatar()->IOhandler()->commandTable();
  std::map<unsigned short,std::vector<std::string> > commands;
  std::vector<std::string> special_commands;
  std::string output;
  std::string foo;
  char buf[64];

  // Find and display all 'special' commands (i.e. mode-specific commands) if we've got any...
  for (std::vector<Command*>::iterator it = table->commands().begin(); it != table->commands().end(); ++it) {
    if (creature->level() >= (*it)->level() && (*it)->special()) {
      special_commands.push_back((*it)->name());
    }
  }
  if (!special_commands.empty()) {
    sort(special_commands.begin(), special_commands.end());
    sprintf(buf, " Commands ({C%u{c) {x==--\n", (unsigned)special_commands.size());
    output.append("\n--== {c").append(table->name()).append(buf).append(Display::formatColumns(special_commands));
  }

  // Find and display all standard commands...
  for (std::vector<Command*>::iterator it = table->commands().begin(); it != table->commands().end(); ++it) {
    if (creature->level() >= (*it)->level() && !(*it)->special() && !(*it)->social()) {
      if ((*it)->enabled()) {
        foo.assign((*it)->name());
      } else {
        foo.assign("{w").append((*it)->name());
      }
      commands[(*it)->level()].push_back(foo);
    }
  }
  for (std::map<unsigned short,std::vector<std::string> >::reverse_iterator m_it = commands.rbegin(); m_it != commands.rend(); ++m_it) {
    sort(m_it->second.begin(), m_it->second.end());
    switch (m_it->first) {
      case CREATOR:   output.append("\n--== {cAdministrator");  break;
      case GOD:       output.append("\n--== {cGod");            break;
      case DEMIGOD:   output.append("\n--== {cDemigod");        break;
      case DUKE:      output.append("\n--== {cDuke");           break;
      case LORD:      output.append("\n--== {cLord");           break;
      case HERO:      output.append("\n--== {cHero");           break;
      default:        output.append("\n--== {cGeneral");        break;
    }
    sprintf(buf, " Commands ({C%u{c) {x==--\n", (unsigned)m_it->second.size());
    output.append(buf).append(Display::formatColumns(m_it->second));
  }

  creature->send(output);
  return true;
}

CmdConnections::CmdConnections(void) {
  name("connections");
  level(GOD);
  addSyntax(0, "");
  brief("Displays the status of all current connections.");
  return;
}

bool CmdConnections::execute(Creature* creature, const std::vector<std::string>& args) {
  estring output;
  char buffer[MAX_BUFFER];

  sprintf(buffer, "  {x%-15s{g%-15s{W%-20s{C%-10s\n", "Name", "Status", "IP Address", "Descriptor");
  output.append(buffer);
  output.append("  ------------------------------------------------------------\n");

  for (std::map<std::string,Avatar*>::const_iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (creature->canSee(it->second) == SEE_NAME) {
      sprintf(buffer, "  {x%-15s{g%-15s{W%-20s{C%-10d\n", it->second->identifiers().shortname().c_str(), it->second->status().string().c_str(), it->second->socket()->getIP().c_str(), it->second->socket()->getFd());
      output.append(buffer);
    }
  }

  creature->send(output);

  return true;
}

CmdColors::CmdColors(void) {
  name("colors");
  addSyntax(0, "");
  brief("Displays the various color codes supported by the game.");
  return;
}

bool CmdColors::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output;
  output.append("Supported color codes:\n\n");
  output.append("  {{r {rdark red      {x{{R {Rbright red{x\n");
  output.append("  {{g {gdark green    {x{{G {Gbright green{x\n");
  output.append("  {{y {ydark yellow   {x{{Y {Ybright yellow{x\n");
  output.append("  {{b {bdark blue     {x{{B {Bbright blue{x\n");
  output.append("  {{m {mdark magenta  {x{{M {Mbright magenta{x\n");
  output.append("  {{c {cdark cyan     {x{{C {Cbright cyan{x\n");
  output.append("  {{w {wdark gray     {x{{W {Wpure white{x\n\n");
  output.append("  {{? {?a single random color{x\n");
  output.append("  Any other characters will be treated to mean \"default\".");
  creature->send(output);
  return true;
}

CmdCrash::CmdCrash(void) {
  name("crash");
  level(CREATOR);
  addSyntax(1, "crash");
  brief("Crashes the mud with a SIGSEG_V.  Why?  Well, you just never know...");
  return;
}

bool CmdCrash::execute(Creature* creature, const std::vector<std::string>& args) {
  if (args[0] == "crash") {
    ((Avatar*)NULL)->send("DIE!!");
  } else {
    avatar()->send(printSyntax());
    return false;
  }
  return true; // never gets here
}

CmdDashboard::CmdDashboard(void) {
  name("dashboard");
  level(CREATOR);
  addSyntax(0, "");
  brief("Displays server status information.");
  return;
}

bool CmdDashboard::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->send("  System time:   {y%s{x\n", World::strtime().c_str());
  avatar()->send("  Engine booted: {y%s{x\n", World::strtime(World::Instance().booted()).c_str());
  avatar()->send("  Engine online: {y%s{x\n", World::realtime(World::now() - World::Instance().booted()).c_str());
  avatar()->send("  Connections:   {y%u{x\n", World::Instance().getAvatars().size());
  avatar()->send("  Jobs in queue: {y%u{x\n", World::Instance().jobsInQueue());
  avatar()->send("  Jobs per turn: {y%u{x\n", World::Instance().jobsPerTurn());
  return true;
}

CmdDelete::CmdDelete(void) {
  name("delete");
  playerOnly(true);
  addSyntax(1, "delete");
  brief("Deletes your player account.  There is no \"undo\".");
  return;
}

bool CmdDelete::execute(Creature* creature, const std::vector<std::string>& args) {
  CmdQuit quit;
  std::vector<std::string> quit_args(1);
  if (args[0] == "delete") {
    if (avatar()->markForDeletion(1)) {
      avatar()->send("You will need to log in to confirm the deletion.\n\n");
      quit.avatar(avatar());
      quit.execute(creature, quit_args);
      return true;
    } else {
      avatar()->send("Something went wrong; your account could not me marked for deletion.\n\n");
      return false;
    }
  } else {
    avatar()->send("You must type \"delete delete\" exactly in order to delete your character.\n\n");
    avatar()->send(printSyntax());
    return false;
  }
}

CmdDemote::CmdDemote(void) {
  name("demote");
  addSyntax(2, "<player> <level>");
  addSyntax(2, "<mob> <level>");
  level(CREATOR);
  brief("Reduces the target to the given level.");
  return;
}

bool CmdDemote::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* target = NULL;
  unsigned level = estring(args[1]);

  // do the checks...
  if ((target = avatar()->findCreature(args[0])) == NULL) {
    avatar()->send("They're not around at the moment.");
    return false;
  }
  if (!avatar()->canAlter(target)) {
    avatar()->send("You can't demote %s.", target->identifiers().shortname().c_str());
    target->send("%s just tried to demote you to level %d.", avatar()->identifiers().shortname().c_str(), level);
    return false;
  }
  if (target->level() < level) {
    avatar()->send("I think you mean to use 'promote' instead...");
    return false;
  }
  if (level == target->level()) {
    avatar()->send("%s is already level %d.", target->identifiers().shortname().c_str(), target->level());
    return false;
  }

  // Give 'em the juice!
  target->level(level);
  target->send("%s has {Rdemoted{x you to level %d!", target->seeName(avatar(), true).c_str(), target->level());
  avatar()->send("%s has been {Rdemoted{x to level %d!", target->identifiers().shortname().c_str(), target->level());

  return true;
}

CmdDescription::CmdDescription(void) {
  name("description");
  playerOnly(true);
  addSyntax(0, "");
  brief("Invokes the Text Editor to change your characters' description.");
  return;
}

bool CmdDescription::execute(Creature* creature, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(avatar());
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Avatar Description"));
  h->getState()["pointer"] = (void*)avatar()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

CmdDisable::CmdDisable(void) {
  name("disable");
  level(CREATOR);
  addSyntax(2, "<command table> <command name>");
  brief("Makes a command (from a certain command table) unusable.");
  return;
}

bool CmdDisable::execute(Creature* creature, const std::vector<std::string>& args) {
  if (World::Instance().toggleCommand(args[0][0], args[1], false)) {
    avatar()->send("%s has been {Rdisabled{x!\n", args[1].c_str());
    return true;
  } else {
    avatar()->send("Sorry - either the table prefix or command name you entered wasn't recognized.\n");
    return false;
  }
}

CmdDisconnect::CmdDisconnect(void) {
  name("disconnect");
  level(CREATOR);
  addSyntax(1, "<player>");
  brief("Terminates the targets connection (their account is saved first)");
  return;
}

bool CmdDisconnect::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = NULL;
  target = World::Instance().findAvatar(args[0]);
  if (!target || avatar()->canSee(target) < SEE_NAME) {
    avatar()->send("They aren't around right now.");
    return false;
  }
  if (target == avatar()) {
    avatar()->send("Wouldn't it be easier to just quit?");
    return false;
  }
  if (!target->save()) {
    avatar()->send("There was a problem saving their profile.");
    return false;
  }
  target->disconnected(true);
  target->send("%s has cut your link - what a buzzkill!", target->seeName(avatar(), true).c_str());
  avatar()->send("You've ripped the game IV from %s's veins!", avatar()->seeName(target).c_str());
  return true;
}

CmdDrop::CmdDrop(void) {
  name("drop");
  addSyntax(1, "<object>");
  brief("Drops an object to the ground.");
  return;
}

bool CmdDrop::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> objects = creature->inventory().searchObjects(args[0]);
  if (objects.empty()) {
    creature->send("You aren't carrying that.");
    return false;
  }
  for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
    if ((*it)->flags().test(OBJECT_NODROP)) {
      creature->send("You can't drop %s.\n", (*it)->identifiers().shortname().c_str());
    } else {
      creature->send("You drop %s.\n", (*it)->identifiers().shortname().c_str());
      creature->room()->send_cond("$p drops $o.\n", creature, *it);
      creature->inventory().remove(*it);
      creature->room()->inventory().add(*it);
    }
  }
  return false;
}

CmdDunce::CmdDunce(void) {
  name("dunce");
  level(GOD);
  addSyntax(1, "<player>");
  brief("Toggles the \"dunce\" status of the target player.  That'll teach 'em.");
  return;
}

bool CmdDunce::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = World::Instance().findAvatar(args[0]);
  char* dunce = strdup(FTAvatarWho::Instance().find(WHO_DUNCE));

  if (!target || avatar()->canSee(target) != SEE_NAME) {
    avatar()->send("They're not around at the moment.");
    return false;
  }
  if (avatar() == target) {
    avatar()->send("You can't %s yourself, sorry.", dunce);
    return false;
  }
  if (!avatar()->canAlter(target)) {
    avatar()->send("You can't do that to %s you %s!", target->identifiers().shortname().c_str(), dunce);
    avatar()->whoFlags().set(WHO_DUNCE);
    target->send("%s tried to call you out for being a %s.  They've been delt their own medicine.", dunce, target->seeName(avatar(), true).c_str());
    return false;
  }

  if (target->whoFlags().test(WHO_DUNCE)) {
    target->whoFlags().clear(WHO_DUNCE);
    target->send("%s has forgiven you for being a %s.", target->seeName(avatar(), true).c_str(), dunce);
    avatar()->send("You've forgiven %s for being a %s.", target->identifiers().shortname().c_str(), dunce);
  } else {
    target->whoFlags().set(WHO_DUNCE);
    target->send("%s called you a %s!", target->seeName(avatar(), true).c_str(), dunce);
    avatar()->send("You just called %s a %s!", target->identifiers().shortname().c_str(), dunce);
  }

  return true;
}

CmdEat::CmdEat(void) {
  name("eat");
  level(DEMIGOD);
  addSyntax(1, "<object>");
  brief("Destroys an object.");
  return;
}

bool CmdEat::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> dead = avatar()->inventory().searchObjects(args[0]);
  if (dead.empty()) {
    avatar()->send("You aren't carrying that.");
    return false;
  }
  for (std::list<Object*>::iterator it = dead.begin(); it != dead.end(); ++it) {
    avatar()->inventory().remove(*it);
    avatar()->send("You eat %s.\n", (*it)->identifiers().shortname().c_str());
    avatar()->room()->send_cond("$p eats $o.\n", avatar(), *it);
    delete *it;
  }
  return true;
}

CmdEmote::CmdEmote(void) {
  name("emote");
  addSyntax(-1, "<action>");
  brief("Displays an arbitrary action preceeded by your name.");
  return;
}

bool CmdEmote::execute(Creature* creature, const std::vector<std::string>& args) {
  switch (args[0][0]) {
    case ',':
    case '\'':
      creature->room()->send("$p$s", creature, (void*)args[0].c_str(), NULL, TO_ALL);
      break;
    default:
      creature->room()->send("$p $s", creature, (void*)args[0].c_str(), NULL, TO_ALL);
  }
  return true;
}

CmdEquipment::CmdEquipment(void) {
  name("equipment");
  addSyntax(0, "");
  brief("Lists the things you're wearing.");
  return;
}

bool CmdEquipment::execute(Creature* creature, const std::vector<std::string>& args) {
  creature->send("You're wearing:");
  if (creature->equipment().objectMap().empty()) {
    creature->send(" nothing!");
  } else {
    creature->send("\n");
    creature->send(creature->equipment().listObjects());
  }
  return true;
}

CmdEnable::CmdEnable(void) {
  name("enable");
  level(CREATOR);
  addSyntax(2, "<command table> <command name>");
  brief("Makes a command (from a certain command table) usable once again.");
  return;
}

bool CmdEnable::execute(Creature* creature, const std::vector<std::string>& args) {
  if (World::Instance().toggleCommand(args[0][0], args[1], true)) {
    creature->send("%s has been {Genabled{x!\n", args[1].c_str());
    return true;
  } else {
    creature->send("Sorry - either the table prefix or command name you entered wasn't recognized.\n");
    return false;
  }
}

CmdExit::CmdExit(void) {
  name("exit");
  playerOnly(true);
  addSyntax(0, "");
  brief("Sends you back out of the current Editor (if you're in one).");
  return;
}

bool CmdExit::execute(Creature* creature, const std::vector<std::string>& args) {
  switch (avatar()->composing().number()) {
    case COMP_NOTE:
      avatar()->replaceIOHandler(new NoteExitIOHandler(creature));
      break;
    default:
      avatar()->popIOHandler();
      break;
  }
  return true;
}

CmdForce::CmdForce(void) {
  name("force");
  level(DEMIGOD);
  addSyntax(-2, "<player> <command>");
  addSyntax(-2, "<mob> <command>");
  brief("Cause the victim to perform the given command.");
  return;
}

bool CmdForce::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* target = NULL;
  bool success = false;

  // Aquire target...
  if ((target = avatar()->findCreature(args[0])) == NULL) {
    avatar()->send("They're not around at the moment.");
    return false;
  }

  // Check permissions...
  if (!avatar()->canAlter(target)) {
    avatar()->send("You can't tell %s what to do!", target->identifiers().shortname().c_str());
    target->send("%s tried to make you \"%s\"", avatar()->identifiers().shortname().c_str(), args[1].c_str());
    return false;
  }

  // Force the command...
  target->send("%s forces you into action.\n", target->seeName(avatar(), true).c_str());
  avatar()->send("%s does as you command.\n", target->identifiers().shortname().c_str());
  if (target->isAvatar()) {
    success = target->IOhandler()->handle(args[1]);
  } else {
    World::Instance().npcIOHandler()->creature(target);
    success = World::Instance().npcIOHandler()->handle(args[1]);
    World::Instance().npcIOHandler()->creature(NULL);
  }

  return success;
}
