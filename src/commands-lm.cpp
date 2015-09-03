
#include <deque>
#include "being.hpp"
#include "command-def.hpp"
#include "exit.hpp"
#include "item-types.hpp"
#include "placement.hpp"
#include "placement-item.hpp"
#include "placement-npc.hpp"
#include "world.hpp"
#include "zone.hpp"

CmdLay::CmdLay(void) {
  name("lay");
  addSyntax(0, "");
  brief("Lay down and rest.");
  return;
}

bool CmdLay::execute(Being* being, const std::vector<std::string>& args) {
  std::string error;
  if (!being->lay(error)) {
    being->send(error);
    return false;
  }
  being->send("You lay down.");
  being->room()->send_cond("$a lays down.", being);
  return true;
}

CmdLearn::CmdLearn(void) {
  name("learn");
  playerOnly(true);
  addSyntax(-1, "<name of skill>");
  addSyntax(-1, "<name of spell>");
  seeAlso("skills");
  seeAlso("spells");
  brief("Use training points to learn an available ability.");
  return;
}

bool CmdLearn::execute(Being* being, const std::vector<std::string>& args) {
  if (args[0].empty()) {
    being->send(printSyntax());
    return false;
  }

  AbilityMap::const_iterator iter = being->klass()->abilities().abilitiesByName().find(args[0]);
  if (iter == being->klass()->abilities().abilitiesByName().end()) {
    being->send("Can't find that ability.");
    return false;
  }

  if (!being->can_learn(iter->second)) {
    being->send("You can't learn that ability just yet.");
    return false;
  }

  being->learn(iter->second, 1);
  being->trains(being->trains() - iter->second->trains());
  being->send("You spend {B%u{x training points learning the {M%s{x ability!\n", iter->second->trains(), iter->second->name().c_str());
  being->send("You now have {B%u{x remaining training points.\n", being->trains());
  return true;
}

CmdPlacement::CmdPlacement(void) {
  name("placement");
  level(Being::DEMIGOD);
  addSyntax(1, "list");
  addSyntax(2, "delete <rule>");
  addSyntax(5, "add npc <vnum> <number> <max>");
  addSyntax(6, "add npc <vnum> <number> <max> <probability>");
  addSyntax(5, "add item <vnum> <number> <max>");
  addSyntax(6, "add item <vnum> <number> <max> <probability>");
  addSyntax(7, "add item <vnum> <number> <max> in <target>[#index]");
  addSyntax(8, "add item <vnum> <number> <max> in <target>[#index] <probability>");
  addSyntax(7, "add item <vnum> <number> <max> on <target>[#index]");
  addSyntax(8, "add item <vnum> <number> <max> on <target>[#index] <probability>");
  addSyntax(7, "add item <vnum> <number> <max> <target>[#index] carry");
  addSyntax(8, "add item <vnum> <number> <max> <target>[#index] carry <probability>");
  addSyntax(7, "add item <vnum> <number> <max> <target>[#index] wear");
  addSyntax(8, "add item <vnum> <number> <max> <target>[#index] wear <probability>");
  brief("Manipulates the reset rules for the room.");
  return;
}

/*"--------Description:\n\
1) View the Load Rules for the current room.\n\
2) Delete the given Load Rule.\n\
3) Create a Load Rule for npc <vnum>.\n\
4) Create a Load Rule for item <vnum>.\n\
5) Like #4, but this item should then be placed in the item specified by vnum <target>, which must be a container.\n\
6) Like #4, but this item should then be placed on the item specified by vnum <target>, which must be a piece of furniture.\n\
7) Like #4, but this item should then be placed in the inventory of the npc specified by vnum <target>.\n\
8) Like #4, but this item should then be worn by the npc specified by vnum <target>.\n\n\
--------Notes:\n\
Load Rules are the rules by which the server automatically loads npcs and\n\
items into the game when zones are \"reset.\"  Every room has it's own list\n\
of Rules that get evaluated when the zone resets. For all Load Rules, <number>\n\
specifies how many clones (items or npcs) should be generated. No more of\n\
these clones will be created after there are <max> copies already anywhere in\n\
the zone. By default the engine will create <number> clones on every single\n\
zone reset until the <max> is reached, however by specifying the optional\n\
[probability] argument (a number between 1 and 99), you can control how often\n\
a clone is made.\n\n\
Additionally, for statements 5 through 8, you may optionally include an index\n\
argument.  If, for example, you are loading two identical pouches (say the\n\
pouch vnum is 934) into a room, and you want a potion to load into the second\n\
pouch, your \"<target>[.index]\" argument should look like \"934#2\"\n\n\
--------Examples:\n\
\"placement add npc 654 1 4\" would specify that 1 of npc 654 should be loaded on every reset, until there are 4 total in the zone.\n\
\"placement add item 3967 5 5\" would specify a rule to load up to five instances of item 3967.\n\
\"placement add item 1234 1 1 5\" would create a rule to load item 1234 approximately 5% of the time.\n\
\"placement add item 4354 1 1 654 carry\" loads one instance of item 4354, and puts it in the inventory of npc 654."*/

bool CmdPlacement::execute(Being* being, const std::vector<std::string>& args) {
  std::deque<std::string> pieces;
  std::string foo;
  std::vector<std::string> bar;
  Room* room = being->room();
  std::list<Placement*>* rules = &(room->placements());
  std::list<Placement*>::iterator it;
  Placement* rule = NULL;
  PlacementNpc* npcRule = NULL;
  PlacementItem* itemRule = NULL;
  CmdPlacement cmdPlacement;
  std::vector<std::string> load_rule_args;
  unsigned rule_number = 0;
  unsigned type = 0;
  unsigned target = 0;
  unsigned number = 0;
  unsigned max = 0;
  unsigned probability = 0;

  // Do we have any arguments?
  if (args.empty()) {
    avatar()->send(printSyntax());
    return false;
  }

  pieces.insert(pieces.begin(), args.begin(), args.end());

  // Check permissions...
  if (!room->zone()->hasPermission(avatar())) {
    avatar()->send("You don't have permissions to the Placements in this room.");
    return false;
  }

  // Provide a listing...
  if (pieces[0] == "list") {
    if (rules->empty()) {
      avatar()->send("There are no Placements defined for this room.");
      return true;
    } else {
      avatar()->send("Placements for Room %lu (%s):\n\n", room->vnum(), room->name().c_str());
      avatar()->send("#  | type |  vnum | number | max | probability | notes\n------------------------------------------------------------------------------\n");
      for (it = rules->begin(), rule_number=1; it != rules->end(); ++it, ++rule_number) {
        switch ((*it)->type()) {
          case Placement::NPC:
            npcRule = (PlacementNpc*)*it;
            avatar()->send("%02u | %4s | %5u |   %2u   |  %2u |   %3u/100   | %s\n", rule_number, npcRule->strType(), npcRule->target(), npcRule->number(), npcRule->max(), npcRule->probability(), "N/A");
            break;
          case Placement::ITEM:
            itemRule = (PlacementItem*)*it;
            avatar()->send("%02u | %4s | %5u |   %2u   |  %2u |   %3u/100   | %s\n", rule_number, itemRule->strType(), itemRule->target(), itemRule->number(), itemRule->max(), itemRule->probability(), itemRule->notes().c_str());
            break;
          default:
            break;
        }
      }
      return true;
    }
  }
  // Deleting a rule...
  else if (pieces[0] == "delete") {
    if (pieces.size() != 2) {
      avatar()->send(printSyntax());
      return false;
    }
    rule_number = estring(pieces[1]);
    if (!rule_number || rule_number > rules->size()) {
      avatar()->send("There is no rule #%u.", rule_number);
      return false;
    } else {
      room->removePlacement(rule_number);
      avatar()->send("Rule #%u has been removed.", rule_number);
      return true;
    }
  }
  // Create a rule...
  else if (pieces[0] == "add") {
    // Do we have required information?
    if (pieces.size() < 3) {
      avatar()->send(printSyntax());
      return false;
    }
    pieces.pop_front();

    // What kind of rule?
    if (pieces[0] == "npc") {
      type = Placement::NPC;
    } else if (pieces[0] == "item") {
      type = Placement::ITEM;
    } else {
      avatar()->send(printSyntax());
      return false;
    }
    pieces.pop_front();

    // Get the target vnum...
    target = estring(pieces[0]);
    pieces.pop_front();

    // How many should we load?
    number = estring(pieces[0]);
    number = number > 10 ? 10 : number;
    pieces.pop_front();

    // Max to load?
    max = estring(pieces[0]);
    max = max > 99 ? 99 : max;
    pieces.pop_front();

    // Construct the Rule...
    switch (type) {
      case Placement::NPC:
        rule = new PlacementNpc();
        if (pieces.empty()) {
          probability = 100;
        } else {
          probability = estring(pieces[0]);
        }
        break;
      case Placement::ITEM:
        rule = new PlacementItem();
        if (pieces.empty()) {
          probability = 100;
          break;
        } else if (pieces.size() == 1) {
          probability = estring(pieces[0]);
          pieces.pop_front();
          break;
        } else if (pieces.size() == 2 || pieces.size() == 3) {
          if (pieces[0] == "in") {
            ((PlacementItem*)rule)->preposition(Placement::IN);
            foo = estring(pieces[1]);
          } else if (pieces[0] == "on") {
            ((PlacementItem*)rule)->preposition(Placement::ON);
            foo = estring(pieces[1]);
          } else if (pieces[1] == "carry") {
            ((PlacementItem*)rule)->preposition(Placement::CARRY);
            foo = estring(pieces[0]);
          } else if (pieces[1] == "wear") {
            ((PlacementItem*)rule)->preposition(Placement::WEAR);
            foo = estring(pieces[0]);
          } else {
            avatar()->send(printSyntax());
            return false;
          }
          pieces.pop_front();
          pieces.pop_front();
          bar = Regex::explode("#", foo);
          switch (bar.size()) {
            case 1:
              ((PlacementItem*)rule)->indirectItem(estring(bar[0]));
              ((PlacementItem*)rule)->indirectItemIndex(1);
              break;
            case 2:
              ((PlacementItem*)rule)->indirectItem(estring(bar[0]));
              ((PlacementItem*)rule)->indirectItemIndex(estring(bar[1]));
              break;
            default:
              avatar()->send(printSyntax());
              return false;
          }
          if (pieces.empty()) {
            probability = 100;
          } else {
            probability = estring(pieces[0]);
            pieces.pop_front();
          }
        } else {
          avatar()->send(printSyntax());
        }
        break;
      default:
        avatar()->send(printSyntax());
        return false;
    }

    // Basic settings...
    rule->vnum(room->vnum());
    rule->room(room);
    rule->type(type);
    rule->target(target);
    rule->number(number);
    rule->max(max);
    rule->probability(probability > 100 ? 100 : probability);
    rules->push_back(rule);
    avatar()->send("Rule created: ");
    load_rule_args.push_back("list");
    cmdPlacement.avatar(avatar());
    cmdPlacement.execute(being, load_rule_args);
    return true;
  } else {
    avatar()->send(printSyntax());
    return false;
  }
}

CmdLock::CmdLock(void) {
  name("lock");
  addSyntax(1, "<direction>");
  brief("Locks a door.");
  return;
}

bool CmdLock::execute(Being* being, const std::vector<std::string>& args) {
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
  if (exit->flag(EXIT_LOCKED)) {
    being->send("It's already locked.");
    return false;
  }
  if (!exit->flag(EXIT_LOCKABLE)) {
    being->send("You can't lock that.");
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

  exit->flag(EXIT_LOCKED, true);
  being->send("You lock the door.");
  being->room()->send_cond("$a locked the way $e.", being, exit);
  return true;
}

CmdLook::CmdLook(void) {
  name("look");
  shortcut("l");
  addSyntax(0, "                  (look around you)");
  addSyntax(1, "<item>          (examine an item)");
  addSyntax(1, "<player>          (examine a player)");
  addSyntax(1, "<npc>             (examine a npc)");
  addSyntax(2, "in <container>    (examine the contents)");
  brief("Examines the room or target item, player, or npc.");
  return;
}

bool CmdLook::execute(Being* being, const std::vector<std::string>& args) {
  std::string output;
  char buffer[Socket::MAX_BUFFER];
  bool has_exits = false;
  Item* container = NULL;
  Being* btarget = NULL;
  Item* otarget = NULL;

  if (args[0].empty()) {
    /*************** looking at the room */
    if (being->level() >= Being::DEMIGOD) {
      sprintf(buffer, "\n\n[{g%lu{x] {%c%s{x\n", being->room()->vnum(), being->room()->terrain()->title(), being->room()->name().c_str());
    } else {
      sprintf(buffer, "\n\n{%c%s{x\n", being->room()->terrain()->title(), being->room()->name().c_str());
    }
    output.append(buffer);
    // Room description...
    sprintf(buffer, "{%c%s{x\n\n", being->room()->terrain()->description(), being->room()->description().c_str());
    output.append(buffer);
    // Exits...
    memset(buffer, 0, Socket::MAX_BUFFER);
    for (unsigned u = NORTH; u <= DOWN; ++u) {
      if (being->room()->exit(u) != NULL) {
        // Bail if we're not supposed to see the exit...
        if (being->room()->exit(u)->flag(EXIT_HIDDEN) && being->level() < Being::DEMIGOD) {
          continue;
        }
        has_exits = true;
        // Open brackets, parentheses, etc...
        if (being->room()->exit(u)->flag(EXIT_HIDDEN)) {
          strcat(buffer, "{W<");
        }
        if (being->room()->exit(u)->flag(EXIT_CLOSED)) {
          strcat(buffer, "{R[");
        }
        // Exit name...
        strcat(buffer, "{C");
        strcat(buffer, Exit::name(u));
        // Close brackets, parentheses, etc...
        if (being->room()->exit(u)->flag(EXIT_CLOSED)) {
          strcat(buffer, "{R]");
        }
        if (being->room()->exit(u)->flag(EXIT_HIDDEN)) {
          strcat(buffer, "{W>");
        }
        strcat(buffer, " ");
      }
    }
    if (has_exits) {
      output.append("[ {WExits{x: {C").append(buffer).append("{x]");
    }
    // Items...
    output.append(1, '\n').append(being->room()->inventory().listItems());
    // Beings...
    for (auto iter : being->room()->beings()) {
      if (iter != being && being->canSee(iter) == Being::SEE_NAME) {
        output.append("{x\n ");
        if (iter->isAvatar()) {
          if (((Avatar*)being)->whoFlags().test(WHO_AFK)) {
            output.append("[{YAFK{x] ");
          }
        }
        output.append(iter->shortname()).append("{x");
        if (iter->isAvatar()) {
          output.append(", a ").append(iter->gender().string()).append(" ").append(iter->race().string()).append(",");
        }
        output.append(" is ").append(iter->position().string()).append(" here.");
      }
    }
    being->send(output);
    return true;
  } else if (args.size() == 1) {
    if ((btarget = being->findBeing(args[0])) != NULL) {
      // looking at a being
      being->send("You look at %s:\n%s{x\n\n", btarget->shortname().c_str(), btarget->description().c_str());
      for (unsigned u = 1; u < Being::WEARLOC_END; ++u) {
        if (btarget->worn(u)) {
          being->send("%s%s{x\n", Being::wearLocName(u), btarget->worn(u)->decorativeShortname().c_str());
        }
      }
      if (btarget->canSee(being) > Being::SEE_NOTHING) {
        btarget->send("%s looks at you.\n", btarget->seeName(being).c_str());
      }
      being->room()->send_cond("$a looks at $c.\n", being, btarget, NULL, Room::TO_NOTVICT);
      return true;
    } else if ((otarget = being->room()->inventory().searchSingleItem(args[0])) != NULL) {
      // looking at an item
      being->send("You look at %s:\n%s{x\n", otarget->shortname().c_str(), otarget->description().c_str());
      being->room()->send_cond("$a looks at $o.\n", being, otarget);
      return true;
    } else {
      being->send("You don't see that here.");
      return false;
    }
  } else if (args.size() == 2) {
    /*************** looking in something */
    if (args[0] == "i" || args[0] == "in") {
      if ((container = being->findItem(args[1])) == NULL) {
        being->send("You don't see that here.");
        return false;
      }
      if (!container->isContainer()) {
        being->send("That's not a container.");
        return false;
      }
      if (container->container()->inventory().itemList().empty()) {
        being->send("%s{x is empty.", container->shortname().c_str());
        return true;
      }
      being->send("You look in %s:\n", container->shortname().c_str());
      being->send(container->container()->inventory().listItems());
      return true;
    } else {
      being->send(printSyntax());
      return false;
    }
  } else {
    being->send(printSyntax());
    return false;
  }
}

CmdMap::CmdMap(void) {
  name("map");
  shortcut("m");
  addSyntax(0, "");
  brief("Displays a map of your surroundings.");
  return;
}

bool CmdMap::execute(Being* being, const std::vector<std::string>& args) {
  std::string output;
  unsigned xsize = 37;
  unsigned ysize = 13;
  unsigned x = 0;
  unsigned y = 0;
  bool success = false;

  // Allocate a 2D array of Room pointers and initialize to NULL...
  Room*** map = new Room**[xsize];
  for (y = 0; y < ysize; ++y) {
    map[y] = new Room*[xsize];
    for (x = 0; x < xsize; ++x) {
      map[y][x] = NULL;
    }
  }

  // Allocate a 2D array of characters and initialize to spaces...
  std::string** display = new std::string*[ysize*2];
  for (y = 0; y < ysize*2; ++y) {
    display[y] = new std::string[xsize*2];
    for (x = 0; x < xsize*2; ++x) {
      display[y][x] = " ";
    }
  }

  // Start a depth-first search of the map, beginning in the current room...
  success = World::search_map(being, map, ysize/2, xsize/2, ysize/2, xsize/2, being->room(), display);

  // Print out our map...
  output.append("{x").append(xsize*2+2, '-').append(1, '\n');
  for (y = ysize*2-1; y > 0; --y) {
    output.append("{x|");
    for (x=0; x < xsize*2; ++x) {
      output.append(display[y][x]);
    }
    output.append("{x|\n");
  }
  output.append("{x").append(xsize*2+2, '-').append(1, '\n');

  if (!success) {
    output.append("There was an error generating this map because the zone isn't square.\nThe output may be garbled.\nPlease contact the Head Builder.");
  }

  being->send(output);
  return true;
}

CmdMoveNorth::CmdMoveNorth(void) {
  name("north");
  shortcut("n");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move to the North.");
  return;
}

bool CmdMoveNorth::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(NORTH, message)) {
    being->move(NORTH);
    return true;
  } else {
    being->send(message);
    return false;
  }
}

CmdMoveEast::CmdMoveEast(void) {
  name("east");
  shortcut("e");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move to the East.");
  return;
}

bool CmdMoveEast::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(EAST, message)) {
    being->move(EAST);
    return true;
  } else {
    being->send(message);
    return false;
  }
}

CmdMoveSouth::CmdMoveSouth(void) {
  name("south");
  shortcut("s");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move to the South.");
  return;
}

bool CmdMoveSouth::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(SOUTH, message)) {
    being->move(SOUTH);
    return true;
  } else {
    being->send(message);
    return false;
  }
}

CmdMoveWest::CmdMoveWest(void) {
  name("west");
  shortcut("w");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move to the West.");
  return;
}

bool CmdMoveWest::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(WEST, message)) {
    being->move(WEST);
    return true;
  } else {
    being->send(message);
    return false;
  }
}

CmdMoveUp::CmdMoveUp(void) {
  name("up");
  shortcut("u");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move up.");
  return;
}

bool CmdMoveUp::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(UP, message)) {
    being->move(UP);
    return true;
  } else {
    being->send(message);
    return false;
  }
}

CmdMoveDown::CmdMoveDown(void) {
  name("down");
  shortcut("d");
  allowedWhile(STANDING | MOUNTED);
  addSyntax(0, "");
  brief("Move Dowm.");
  return;
}

bool CmdMoveDown::execute(Being* being, const std::vector<std::string>& args) {
  std::string message;
  if (being->canMove(DOWN, message)) {
    being->move(DOWN);
    return true;
  } else {
    being->send(message);
    return false;
  }
}
