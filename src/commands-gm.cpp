
#include <deque>
#include "area.h"
#include "avatar.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "flagTable.h"
#include "group.h"
#include "io-handler.h"
#include "loadRule.h"
#include "loadRuleMob.h"
#include "loadRuleItem.h"
#include "mob.h"
#include "item-container.h"
#include "room.h"
#include "world.h"

CmdGecho::CmdGecho(void) {
  name("gecho");
  level(Being::GOD);
  addSyntax(-1, "<message>");
  brief("Broadcasts a message to all connected players in.");
  return;
}

bool CmdGecho::execute(Being* being, const std::vector<std::string>& args) {
  for (std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (it->second->isConnected()) {
      it->second->send("{%c%s{x", avatar()->gechoColor(), args[0].c_str());
    }
  }
  return true;
}

CmdGet::CmdGet(void) {
  name("get");
  addSyntax(1, "<item>");
  addSyntax(2, "<item> <container>");
  brief("Picks up an item.");
  return;
}

bool CmdGet::execute(Being* being, const std::vector<std::string>& args) {
  std::list<Item*> items;
  Item* container = NULL;

  if (args.size() == 1) {
    /* look for items lying in the room */
    items = being->room()->inventory().searchItems(args[0]);
    if (items.empty()) {
      being->send("There aren't any of those here.");
      return false;
    }
    for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
      if ((*it)->flags().test(ITEM_NOGET)) {
        being->send("You can't get %s{x.\n", (*it)->identifiers().shortname().c_str());
      } else {
        being->send("You get %s.\n", (*it)->identifiers().shortname().c_str());
        being->room()->send_cond("$p gets $o.\n", being, *it);
        being->room()->inventory().remove(*it);
        being->inventory().add(*it);
      }
    }
  } else if (args.size() == 2) {
    /* look for items in a container */
    if ((container = being->findItem(args[1])) == NULL) {
      being->send("You can't find that.");
      return false;
    }
    if (!container->isContainer()) {
      being->send("That's not a container.");
      return false;
    }
    items = container->container()->inventory().searchItems(args[0]);
    if (items.empty()) {
      being->send("There aren't any of those in %s{x.", container->identifiers().shortname().c_str());
      return false;
    }
    for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
      // transfer each item
      container->container()->inventory().remove(*it);
      being->inventory().add(*it);
      being->send("You get %s{x from %s{x.\n", (*it)->identifiers().shortname().c_str(), container->identifiers().shortname().c_str());
      being->room()->send_cond("$p gets $o{x from $O{x.\n", being, *it, container);
    }
  }

  return true;
}

CmdGive::CmdGive(void) {
  name("give");
  addSyntax(2, "<item> <player>");
  addSyntax(2, "<item> <mob>");
  brief("Gives an item to the target.");
  return;
}

bool CmdGive::execute(Being* being, const std::vector<std::string>& args) {
  std::list<Item*> items;
  Being* target = NULL;
  Item* item = NULL;

  if ((target = being->findBeing(args[1])) == NULL) {
    being->send("They're not around at the moment.");
    return false;
  }
  if ((items = being->inventory().searchItems(args[0])).empty()) {
    being->send("You don't have that.");
    return false;
  }
  if (items.empty()) {
    being->send("You don't have that.");
    return false;
  }
  for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
    item = *it;
    if (item->flags().test(ITEM_NODROP)) {
      being->send("You can't let go of %s.\n", item->identifiers().shortname().c_str());
    } else {
      being->inventory().remove(item);
      target->inventory().add(item);
      being->send("You give %s to %s.\n", item->identifiers().shortname().c_str(), target->identifiers().shortname().c_str());
      target->send("%s gives you %s.\n", target->seeName(being, true).c_str(), item->identifiers().shortname().c_str());
      being->room()->send_cond("$p gives $c $O.\n", being, target, item, Room::TO_NOTVICT);
    }
  }

  return true;
}

CmdGoto::CmdGoto(void) {
  name("goto");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>        (target room)");
  addSyntax(1, "<player>      (target player)");
  addSyntax(1, "<mob>         (target mob)");
  brief("Transports you to the target.");
  return;
}

bool CmdGoto::execute(Being* being, const std::vector<std::string>& args) {
  Being* target = NULL;
  Room* room = NULL;
  CmdLook look;
  std::vector<std::string> look_args(1);

  if (Regex::match("^[0-9]+$", args[0])) {
    if ((room = World::Instance().findRoom(estring(args[0]))) == NULL) {
      avatar()->send("That room doesn't exist.");
      return false;
    }
  } else {
    target = World::Instance().findBeing(args[0]);
    if (!target || avatar()->canSee(target) < Being::SEE_NAME) {
      avatar()->send("They aren't around right now.");
      return false;
    }
    room = target->room();
  }

  if (room == avatar()->room()) {
    being->send("You're already there.");
    return false;
  }

  avatar()->room()->remove(avatar());
  avatar()->room()->send_cond(avatar()->poofout(), avatar());
  room->add(avatar());
  room->send_cond(avatar()->poofin(), avatar());
  avatar()->room(room);
  look.avatar(avatar());
  look.execute(avatar(), look_args);
  return true;
}

CmdGroup::CmdGroup(void) {
  name("group");
  addSyntax(0, "");
  addSyntax(1, "<player>");
  brief("Manage group membership.");
  return;
}

bool CmdGroup::execute(Being* being, const std::vector<std::string>& args) {
  std::string name;
  Group* this_group = being->group();
  Group* new_group  = NULL;
  Being* target  = NULL;
  Being* leader  = NULL;

  name = args[0];
  // Empty name - list the group stats.
  if (name.empty()) {
    being->send("+----------------------------------------------------------------------------+\n");
    being->send("| Group Leader: {B%-20s{x {YLEVEL   {GHEALTH      {CMANA       {MSTAMINA  {x|\n", this_group->leader()->identifiers().shortname().c_str());
    being->send("+----------------------------------------------------------------------------+\n");
    for (std::set<Being*>::iterator it = this_group->members().begin(); it != this_group->members().end(); ++it) {
      being->send("| {W%-20s{x                {Y%3d{x  {G%4d{x/{g%-4u  {C%4u{x/{c%-4u       {M%3u{x    |\n",
        (*it)->identifiers().shortname().c_str(),
        (*it)->level(), (*it)->health(), (*it)->maxHealth(), (*it)->mana(), (*it)->maxMana(), (*it)->stamina()
     );
    }
    being->send("+----------------------------------------------------------------------------+\n");
  }
  // Got a name - look for the player and join, if possible.
  else {
    // Find the target.
    if ((target = being->findBeing(args[0])) == NULL) {
      being->send("You don't see them here.");
      return false;
    }
    // Make sure you're following the leader of the group (for example, if Alice is following
    // Bob, and you follow Alice, you actually want to group with Bob directly).
    new_group = target->group();
    leader    = new_group->leader();
    // Can't follow yourself.
    if (target == being) {
      being->send("Follow yourself? How would that work?");
      return false;
    }
    // Same group, silly.
    if (leader == being) {
      being->send("You're already in the same group.");
      return false;
    }
    // Everyone in your group should follow the new leader, otherwise we have to either disband
    // your current group, or arbitrarily select a new leader. Both of those options seem suboptimal
    // so instead we merge the two groups under the new leader. This same logic also works for the
    // simplified case where you're solo.
    std::set<Being*> current_members(this_group->members());
    for (std::set<Being*>::iterator iter = current_members.begin(); iter != current_members.end(); ++iter) {
      this_group->remove_member(*iter);
      new_group->add_member(*iter);
      (*iter)->group(new_group);
      (*iter)->send("You are now grouped with %s.\n", leader->identifiers().shortname().c_str());
      leader->send("%s has joined your group.\n", (*iter)->identifiers().shortname().c_str());
      (*iter)->room()->send_cond("$p is now grouped with $c.\n", *iter, leader, NULL, Room::TO_NOTVICT);
    }
  }
  return true;
}

CmdHeal::CmdHeal(void) {
  name("heal");
  playerOnly(true);
  level(Being::DEMIGOD);
  addSyntax(1, "<target>");
  addSyntax(1, "all");
  brief("Restores hp/mp/mv to the target, or all players.");
  return;
}

bool CmdHeal::execute(Being* being, const std::vector<std::string>& args) {
  Being* target = NULL;
  if (args[0] == "all") {
    for (std::map<std::string, Avatar*>::iterator iter = World::Instance().getAvatars().begin(); iter != World::Instance().getAvatars().end(); ++iter) {
      iter->second->heal();
      if (iter->second == being) continue;
      iter->second->send(iter->second->seeName(being, true));
      iter->second->send(" has healed you.\n");
    }
    being->send("You have healed the world.\n");
  } else {
    if ((target = World::Instance().findBeing(args[0])) == NULL) {
      avatar()->send("They're not around at the moment.");
      return false;
    }
    target->heal();
    if (target == being) {
      target->send("You heal yourself.\n");
    } else {
      target->send(target->seeName(being, true));
      target->send(" has healed you.\n");
      being->send("You have healed %s.\n", target->identifiers().shortname().c_str());
    }
  }
  return true;
}

CmdHelp::CmdHelp(void) {
  name("help");
  playerOnly(true);
  addSyntax(1, "<command>");
  brief("Provides information about commands.");
  return;
}

bool CmdHelp::execute(Being* being, const std::vector<std::string>& args) {
  Command* command = NULL;
  if ((command = avatar()->IOhandler()->commandTable()->find(args[0])) == NULL || command->level() > avatar()->level()) {
    avatar()->send("%s is not a valid command.", args[0].c_str());
    return false;
  }
  avatar()->send(command->printSyntax());
  return true;
}

CmdIdentify::CmdIdentify(void) {
  name("identify");
  level(Being::DEMIGOD);
  addSyntax(1, "<item>");
  addSyntax(1, "<mob>");
  brief("Displays diagnostic information on the target.");
  return;
}

bool CmdIdentify::execute(Being* being, const std::vector<std::string>& args) {
  Item* i = NULL;
  Being* b = NULL;
  std::string output;
  if ((b = avatar()->findBeing(args[0])) == NULL) {
    // check for items matching the arg
    if ((i = avatar()->findItem(args[0])) == NULL) {
      avatar()->send("You don't see that here.");
      return false;
    } else {
      avatar()->send(i->printStatus());
    }
  } else {
    if (b->isMob()) {
      avatar()->send(Mob::getInformation((Mob*)b));
    } else {
      avatar()->send("Sorry - 'identify' doesn't work on players yet");
      return false;
    }
  }
  return true;
}

CmdIedit::CmdIedit(void) {
  name("iedit");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  addSyntax(2, "create <vnum>");
  brief("Invokes the Item Editor.");
  return;
}

bool CmdIedit::execute(Being* being, const std::vector<std::string>& args) {
  std::map<unsigned long,Item*>::iterator it;
  Area* area = NULL;
  Item* item = NULL;
  unsigned long vnum = 0;

  if (args.size() == 1) {
    vnum = estring(args[0]);
    // Get the area...
    if ((area = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!area->hasPermission(avatar())) {
      avatar()->send("You don't have permissions to that item.");
      return false;
    }
    // Make sure the Item exists...
    if ((it = area->items().find(vnum)) == area->items().end()) {
      avatar()->send("That item doesn't exist.");
      return false;
    }
    // Make sure no one else is editing the item...
    for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
      if (a_it->second->mode().number() == MODE_IEDIT && a_it->second->iedit() == it->second) {
        avatar()->send("Sorry, %s is currently editing %s (item %lu).", avatar()->seeName(((Being*)a_it->second)).c_str(), it->second->identifiers().shortname().c_str(), it->second->vnum());
        return false;
      }
    }
    // All looks well; send them to iedit...
    avatar()->iedit(it->second);
    avatar()->pushIOHandler(new IeditIOHandler(avatar()));
    avatar()->send("You're editing item %lu.", avatar()->iedit()->vnum());
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
    if (!area->hasPermission((Avatar*)being)) {
      avatar()->send("You don't have permissions to that item.");
      return false;
    }
    // Make sure the Item doesn't already exist...
    if ((it = area->items().find(vnum)) != area->items().end()) {
      avatar()->send("That item already exists.");
      return false;
    }
    // Everything checks out; let's make us a new Item!
    item = new Item();
    item->vnum(vnum);
    area->items()[item->vnum()] = item;
    avatar()->send("Item %lu created successfully.", item->vnum());
    avatar()->mode().set(MODE_IEDIT);
    avatar()->iedit(item);
    avatar()->pushIOHandler(new IeditIOHandler(being));
    return true;
  }

  avatar()->send(printSyntax());
  return false;
}

CmdIlist::CmdIlist(void) {
  name("ilist");
  level(Being::DEMIGOD);
  addSyntax(1, "<areaID>                       (list all Items in the area)");
  addSyntax(2, "<first vnum> <last vnum>       (list all Items in the vnum range)");
  addSyntax(1, "<keyword>                      (list all Items by keyword)");
  addSyntax(1, "/<regex>                       (list all Items matching the PCRE)");
  return;
}

bool CmdIlist::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args = args;
  std::vector<Item*> items;
  Area* area = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  if (mutable_args.size() == 1) {
    if (Regex::match("^[0-9]+$", mutable_args[0])) {
      // We got an areaID...
      if ((area = World::Instance().findArea(estring(mutable_args[0]))) == NULL) {
        being->send("That area couldn't be found.");
        return false;
      }
      for (std::map<unsigned long,Item*>::iterator o_it = area->items().begin(); o_it != area->items().end(); ++o_it) {
        items.push_back(o_it->second);
      }
    } else {
      if (mutable_args[0][0] == '/') {
        mutable_args[0].erase(0, 1);
        // This search is a regex...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Item*>::iterator o_it = (*a_it)->items().begin(); o_it != (*a_it)->items().end(); ++o_it) {
            if (o_it->second->identifiers().matchesKeyword(mutable_args[0])) {
              items.push_back(o_it->second);
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Item*>::iterator o_it = (*a_it)->items().begin(); o_it != (*a_it)->items().end(); ++o_it) {
            if (o_it->second->identifiers().matchesKeyword(search)) {
              items.push_back(o_it->second);
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
    // Grab the rooms...
    for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
      for (std::map<unsigned long,Item*>::iterator o_it = (*a_it)->items().begin(); o_it != (*a_it)->items().end(); ++o_it) {
        if (o_it->second->vnum() >= low && o_it->second->vnum() <= high) {
          items.push_back(o_it->second);
        }
      }
    }
  } else {
    being->send(printSyntax());
    return false;
  }

  if (items.empty()) {
    being->send("No matches for \"%s\"", mutable_args[0].c_str());
    return false;
  }

  output.append(" [{y vnum{x] {gname{x\n -------------------\n");
  for (std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
    sprintf(buffer, " [{y%5lu{x] %s{x\n", (*it)->vnum(), (*it)->identifiers().shortname().c_str());
    output.append(buffer);
  }

  being->send(output);
  return true;
}

CmdIload::CmdIload(void) {
  name("iload");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  brief("Fabricates an Item.");
}

bool CmdIload::execute(Being* being, const std::vector<std::string>& args) {
  unsigned long vnum = estring(args[0]);
  std::map<unsigned long,Item*>::iterator o_it;
  Item* item = NULL;

  for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
    if ((o_it = (*a_it)->items().find(vnum)) != (*a_it)->items().end()) {
      item = new Item(*o_it->second);
      if (item == NULL) {
        being->send("Oload failed.");
        return false;
      }
      if (item->identifiers().shortname().empty() || item->identifiers().longname().empty() || item->identifiers().getKeywords().empty()) {
        avatar()->send("Sorry; that item isn't complete yet.");
        return false;
      }
      World::Instance().insert(item);
      being->inventory().add(item);
      being->send("You load %s.", item->identifiers().shortname().c_str());
      being->room()->send_cond("$p has created $o.", being, item);
      return true;
    }
  }

  being->send("There is no item of that vnum.");
  return false;
}

CmdIncognito::CmdIncognito(void) {
  name("incognito");
  level(Being::GOD);
  addSyntax(0, "");
  brief("Makes an immortal undetectable.");
  return;
}

bool CmdIncognito::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->adminFlags().toggle(ADMIN_INCOGNITO);
  if (avatar()->adminFlags().test(ADMIN_INCOGNITO)) {
    avatar()->send("You disappear.");
  } else {
    avatar()->send("You fade into existence.");
    avatar()->room()->send_cond("$p fades into existence.", avatar());
  }
  return true;
}

CmdInventory::CmdInventory(void) {
  name("inventory");
  addSyntax(0, "");
  brief("Displays the items currently carried.");
  return;
}

bool CmdInventory::execute(Being* being, const std::vector<std::string>& args) {
  std::string output;
  if (being->inventory().itemList().empty()) {
    being->send("You aren't carrying anything at the moment.");
    return true;
  }
  being->send("You are carrying:\n");
  being->send(being->inventory().listItems().c_str());
  return true;
}

CmdKill::CmdKill(void) {
  name("kill");
  addSyntax(1, "<target>");
  brief("Attack a player or mob, initiating combat.");
  return;
}

bool CmdKill::execute(Being* being, const std::vector<std::string>& args) {
  Being* target = NULL;
  if ((target = being->findBeing(args[0])) == NULL) {
    being->send("But they're not even here right now!");
    return false;
  } else if (target == being) {
    being->send("Your sense of honor precludes suicide.");
    return false;
  } else if (!being->deplete_stamina(1)) {
    return false;
  } else {
    being->send("You attack %s!\n", target->name());
    target->send("%s attacks you!\n", being->name());
    being->room()->send_cond("$p attacks $c!\n", being, target, NULL, Room::TO_NOTVICT);
    being->add_opponent(target);
  }
  return true;
}

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
  being->room()->send_cond("$p lays down.", being);
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

CmdLoadRule::CmdLoadRule(void) {
  name("loadrule");
  level(Being::DEMIGOD);
  addSyntax(1, "list");
  addSyntax(2, "delete <rule>");
  addSyntax(5, "add mob <vnum> <number> <max>");
  addSyntax(6, "add mob <vnum> <number> <max> <probability>");
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
3) Create a Load Rule for mob <vnum>.\n\
4) Create a Load Rule for item <vnum>.\n\
5) Like #4, but this item should then be placed in the item specified by vnum <target>, which must be a container.\n\
6) Like #4, but this item should then be placed on the item specified by vnum <target>, which must be a piece of furniture.\n\
7) Like #4, but this item should then be placed in the inventory of the mob specified by vnum <target>.\n\
8) Like #4, but this item should then be worn by the mob specified by vnum <target>.\n\n\
--------Notes:\n\
Load Rules are the rules by which the server automatically loads mobs and\n\
items into the game when areas are \"reset.\"  Every room has it's own list\n\
of Rules that get evaluated when the area resets. For all Load Rules, <number>\n\
specifies how many clones (items or mobs) should be generated. No more of\n\
these clones will be created after there are <max> copies already anywhere in\n\
the area. By default the engine will create <number> clones on every single\n\
area reset until the <max> is reached, however by specifying the optional\n\
[probability] argument (a number between 1 and 99), you can control how often\n\
a clone is made.\n\n\
Additionally, for statements 5 through 8, you may optionally include an index\n\
argument.  If, for example, you are loading two identical pouches (say the\n\
pouch vnum is 934) into a room, and you want a potion to load into the second\n\
pouch, your \"<target>[.index]\" argument should look like \"934#2\"\n\n\
--------Examples:\n\
\"loadrule add mob 654 1 4\" would specify that 1 of mob 654 should be loaded on every reset, until there are 4 total in the area.\n\
\"loadrule add item 3967 5 5\" would specify a rule to load up to five instances of item 3967.\n\
\"loadrule add item 1234 1 1 5\" would create a rule to load item 1234 approximately 5% of the time.\n\
\"loadrule add item 4354 1 1 654 carry\" loads one instance of item 4354, and puts it in the inventory of mob 654."*/

bool CmdLoadRule::execute(Being* being, const std::vector<std::string>& args) {
  std::deque<std::string> pieces;
  std::string foo;
  std::vector<std::string> bar;
  Room* room = being->room();
  std::list<LoadRule*>* rules = &(room->loadRules());
  std::list<LoadRule*>::iterator it;
  LoadRule* rule = NULL;
  LoadRuleMob* mobRule = NULL;
  LoadRuleItem* itemRule = NULL;
  CmdLoadRule cmdLoadRule;
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
  if (!room->area()->hasPermission(avatar())) {
    avatar()->send("You don't have permissions to the LoadRules in this room.");
    return false;
  }

  // Provide a listing...
  if (pieces[0] == "list") {
    if (rules->empty()) {
      avatar()->send("There are no LoadRules defined for this room.");
      return true;
    } else {
      avatar()->send("LoadRules for Room %lu (%s):\n\n", room->vnum(), room->name().c_str());
      avatar()->send("#  | type |  vnum | number | max | probability | notes\n------------------------------------------------------------------------------\n");
      for (it = rules->begin(), rule_number=1; it != rules->end(); ++it, ++rule_number) {
        switch ((*it)->type()) {
          case LoadRule::MOB:
            mobRule = (LoadRuleMob*)*it;
            avatar()->send("%02u | %4s | %5u |   %2u   |  %2u |   %3u/100   | %s\n", rule_number, mobRule->strType(), mobRule->target(), mobRule->number(), mobRule->max(), mobRule->probability(), "N/A");
            break;
          case LoadRule::ITEM:
            itemRule = (LoadRuleItem*)*it;
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
      room->removeLoadRule(rule_number);
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
    if (pieces[0] == "mob") {
      type = LoadRule::MOB;
    } else if (pieces[0] == "item") {
      type = LoadRule::ITEM;
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
      case LoadRule::MOB:
        rule = new LoadRuleMob();
        if (pieces.empty()) {
          probability = 100;
        } else {
          probability = estring(pieces[0]);
        }
        break;
      case LoadRule::ITEM:
        rule = new LoadRuleItem();
        if (pieces.empty()) {
          probability = 100;
          break;
        } else if (pieces.size() == 1) {
          probability = estring(pieces[0]);
          pieces.pop_front();
          break;
        } else if (pieces.size() == 2 || pieces.size() == 3) {
          if (pieces[0] == "in") {
            ((LoadRuleItem*)rule)->preposition(LoadRule::IN);
            foo = estring(pieces[1]);
          } else if (pieces[0] == "on") {
            ((LoadRuleItem*)rule)->preposition(LoadRule::ON);
            foo = estring(pieces[1]);
          } else if (pieces[1] == "carry") {
            ((LoadRuleItem*)rule)->preposition(LoadRule::CARRY);
            foo = estring(pieces[0]);
          } else if (pieces[1] == "wear") {
            ((LoadRuleItem*)rule)->preposition(LoadRule::WEAR);
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
              ((LoadRuleItem*)rule)->indirectItem(estring(bar[0]));
              ((LoadRuleItem*)rule)->indirectItemIndex(1);
              break;
            case 2:
              ((LoadRuleItem*)rule)->indirectItem(estring(bar[0]));
              ((LoadRuleItem*)rule)->indirectItemIndex(estring(bar[1]));
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
    cmdLoadRule.avatar(avatar());
    cmdLoadRule.execute(being, load_rule_args);
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
  being->room()->send_cond("$p locked the way $e.", being, exit);
  return true;
}

CmdLook::CmdLook(void) {
  name("look");
  shortcut("l");
  addSyntax(0, "                  (look around you)");
  addSyntax(1, "<item>          (examine an item)");
  addSyntax(1, "<player>          (examine a player)");
  addSyntax(1, "<mob>             (examine a mob)");
  addSyntax(2, "in <container>    (examine the contents)");
  brief("Examines the room or target item, player, or mob.");
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
        output.append(iter->identifiers().shortname()).append("{x");
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
      being->send("You look at %s:\n%s{x\n\n", btarget->identifiers().shortname().c_str(), btarget->identifiers().description().c_str());
      for (unsigned u = 1; u < Being::WEARLOC_END; ++u) {
        if (btarget->worn(u)) {
          being->send("%s%s{x\n", Being::wearLocName(u), btarget->worn(u)->decorativeShortname().c_str());
        }
      }
      if (btarget->canSee(being) > Being::SEE_NOTHING) {
        btarget->send("%s looks at you.\n", btarget->seeName(being).c_str());
      }
      being->room()->send_cond("$p looks at $c.\n", being, btarget, NULL, Room::TO_NOTVICT);
      return true;
    } else if ((otarget = being->room()->inventory().searchSingleItem(args[0])) != NULL) {
      // looking at an item
      being->send("You look at %s:\n%s{x\n", otarget->identifiers().shortname().c_str(), otarget->identifiers().description().c_str());
      being->room()->send_cond("$p looks at $o.\n", being, otarget);
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
        being->send("%s{x is empty.", container->identifiers().shortname().c_str());
        return true;
      }
      being->send("You look in %s:\n", container->identifiers().shortname().c_str());
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
    output.append("There was an error generating this map because the area isn't square.\nThe output may be garbled.\nPlease contact the Head Builder.");
  }

  being->send(output);
  return true;
}

CmdMedit::CmdMedit(void) {
  name("medit");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  addSyntax(2, "create <vnum>");
  brief("Launches the Mob Editor.");
  return;
}

bool CmdMedit::execute(Being* being, const std::vector<std::string>& args) {
  std::map<unsigned long,Mob*>::iterator it;
  Area* area = NULL;
  Mob* mob = NULL;
  unsigned long vnum = 0;

  if (args.size() == 1) {
    vnum = estring(args[0]);
    // Get the area...
    if ((area = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!area->hasPermission(avatar())) {
      avatar()->send("You don't have permissions to that item.");
      return false;
    }
    // Make sure the Item exists...
    if ((it = area->mobs().find(vnum)) == area->mobs().end()) {
      avatar()->send("That mob doesn't exist.");
      return false;
    }
    // Make sure no one else is editing the item...
    for (std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it) {
      if (a_it->second->mode().number() == MODE_MEDIT && a_it->second->medit() == it->second) {
        avatar()->send("Sorry, %s is currently editing %s (mob %lu).", avatar()->seeName(((Being*)a_it->second)).c_str(), it->second->identifiers().shortname().c_str(), it->second->vnum());
        return false;
      }
    }
    // All looks well; send them to medit...
    avatar()->medit(it->second);
    avatar()->pushIOHandler(new MeditIOHandler(avatar()));
    avatar()->send("You're editing mob %lu.", avatar()->medit()->vnum());
    return true;
  } else if (args.size() == 2) {
    if (!Regex::strPrefix(args[0], "create")) {
      avatar()->send(printSyntax());
      return false;
    }
    vnum = estring(args[1]);
    // Get the area...
    if ((area = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!area->hasPermission(avatar())) {
      avatar()->send("You don't have permissions to that mob.");
      return false;
    }
    // Make sure the Mob doesn't already exist...
    if ((it = area->mobs().find(vnum)) != area->mobs().end()) {
      avatar()->send("That mob already exists.");
      return false;
    }
    // Everything checks out; let's make us a new Mob!
    if ((mob = Mob::create(area, vnum)) == NULL) {
      avatar()->send("There was an error creating the mob.");
      return false;
    }
    avatar()->send("Mob %lu created successfully.", mob->vnum());
    avatar()->mode().set(MODE_MEDIT);
    avatar()->medit(mob);
    avatar()->pushIOHandler(new MeditIOHandler(avatar()));
    return true;
  }

  return false;
}

CmdMlist::CmdMlist(void) {
  name("mlist");
  level(Being::DEMIGOD);
  addSyntax(1, "<areaID>                       (list all Mobs in the area)");
  addSyntax(2, "<first vnum> <last vnum>       (list all Mobs in the vnum range)");
  addSyntax(1, "<keyword>                      (list all Mobs by keyword)");
  addSyntax(1, "/<regex>                       (list all Mobs matching the PCRE)");
  return;
}

bool CmdMlist::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args(args);
  std::vector<Mob*> mobs;
  Area* area = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  if (mutable_args.size() == 1) {
    if (Regex::match("^[0-9]+$", mutable_args[0])) {
      // We got an areaID...
      if ((area = World::Instance().findArea(estring(mutable_args[0]))) == NULL) {
        being->send("That area couldn't be found.");
        return false;
      }
      for (std::map<unsigned long,Mob*>::iterator m_it = area->mobs().begin(); m_it != area->mobs().end(); ++m_it) {
        mobs.push_back(m_it->second);
      }
    } else {
      if (mutable_args[0][0] == '/') {
        mutable_args[0].erase(0, 1);
        // This search is a regex...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it) {
            if (m_it->second->identifiers().matchesKeyword(mutable_args[0])) {
              mobs.push_back(m_it->second);
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
          for (std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it) {
            if (m_it->second->identifiers().matchesKeyword(search)) {
              mobs.push_back(m_it->second);
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
    // Grab the mobs...
    for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
      for (std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it) {
        if (m_it->second->vnum() >= low && m_it->second->vnum() <= high) {
          mobs.push_back(m_it->second);
        }
      }
    }
  }

  if (mobs.empty()) {
    being->send("No matches for \"%s\"", mutable_args[0].c_str());
    return false;
  }

  output.append(" [{y vnum{x] {gname{x\n -------------------\n");
  for (std::vector<Mob*>::iterator it = mobs.begin(); it != mobs.end(); ++it) {
    sprintf(buffer, " [{y%5u{x] %s{x\n", (*it)->vnum(), (*it)->identifiers().shortname().c_str());
    output.append(buffer);
  }

  being->send(output);
  return true;
}

CmdMload::CmdMload(void) {
  name("mload");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>");
  brief("Incarnates a Mob.");
}

bool CmdMload::execute(Being* being, const std::vector<std::string>& args) {
  unsigned long vnum = estring(args[0]);
  std::map<unsigned long,Mob*>::iterator m_it;
  Mob* mob = NULL;

  for (std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it) {
    if ((m_it = (*a_it)->mobs().find(vnum)) != (*a_it)->mobs().end()) {
      mob = Mob::create(m_it->second, being->room());
      if (mob == NULL) {
        being->send("Mload failed. Don't you feel stupid?");
        return false;
      }
      if (mob->identifiers().shortname().empty() || mob->identifiers().longname().empty() || mob->identifiers().getKeywords().empty()) {
        avatar()->send("Sorry; that mob isn't complete yet.");
        return false;
      }
      World::Instance().insert(mob);
      being->room()->add(mob);
      mob->room(being->room());
      mob->mobilize();
      being->send("You load %s.", mob->identifiers().shortname().c_str());
      being->room()->send_cond("$p has created $c.", being, mob);
      return true;
    }
  }

  being->send("There is no mob of that vnum.");
  return false;
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
