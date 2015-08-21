
#include "being.hpp"
#include "command-def.hpp"
#include "item-types.hpp"
#include "os.hpp"
#include "world.hpp"
#include "zone.hpp"

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
        being->room()->send_cond("$a gets $o.\n", being, *it);
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
      being->room()->send_cond("$a gets $o{x from $O{x.\n", being, *it, container);
    }
  }

  return true;
}

CmdGive::CmdGive(void) {
  name("give");
  addSyntax(2, "<item> <player>");
  addSyntax(2, "<item> <npc>");
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
      being->room()->send_cond("$a gives $c $O.\n", being, target, item, Room::TO_NOTVICT);
    }
  }

  return true;
}

CmdGoto::CmdGoto(void) {
  name("goto");
  level(Being::DEMIGOD);
  addSyntax(1, "<vnum>        (target room)");
  addSyntax(1, "<player>      (target player)");
  addSyntax(1, "<npc>         (target npc)");
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
      (*iter)->room()->send_cond("$a is now grouped with $c.\n", *iter, leader, NULL, Room::TO_NOTVICT);
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
  addSyntax(1, "<player>");
  addSyntax(1, "<npc>");
  brief("Displays diagnostic information on the target.");
  return;
}

bool CmdIdentify::execute(Being* being, const std::vector<std::string>& args) {
  Item* i  = nullptr;
  Being* b = nullptr;
  std::string output;
  if ((b = avatar()->findBeing(args[0])) == nullptr) {
    // check for items matching the arg
    if ((i = avatar()->findItem(args[0])) == nullptr) {
      avatar()->send("You don't see that here.");
      return false;
    } else {
      avatar()->send(i->printInformation());
    }
  } else {
    avatar()->send(b->printInformation());
    return true;
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
  Zone* zone = NULL;
  Item* item = NULL;
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
    if ((it = zone->items().find(vnum)) == zone->items().end()) {
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
    // Get the zone...
    if ((zone = World::Instance().lookup(vnum)) == NULL) {
      avatar()->send("That vnum doesn't exist.");
      return false;
    }
    // Check permissions...
    if (!zone->hasPermission((Avatar*)being)) {
      avatar()->send("You don't have permissions to that item.");
      return false;
    }
    // Make sure the Item doesn't already exist...
    if ((it = zone->items().find(vnum)) != zone->items().end()) {
      avatar()->send("That item already exists.");
      return false;
    }
    // Everything checks out; let's make us a new Item!
    item = new Item();
    item->vnum(vnum);
    zone->items()[item->vnum()] = item;
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
  addSyntax(1, "<zoneID>                       (list all Items in the zone)");
  addSyntax(2, "<first vnum> <last vnum>       (list all Items in the vnum range)");
  addSyntax(1, "<keyword>                      (list all Items by keyword)");
  addSyntax(1, "/<regex>                       (list all Items matching the PCRE)");
  return;
}

bool CmdIlist::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args = args;
  std::vector<Item*> items;
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
      for (std::map<unsigned long,Item*>::iterator o_it = zone->items().begin(); o_it != zone->items().end(); ++o_it) {
        items.push_back(o_it->second);
      }
    } else {
      if (mutable_args[0][0] == '/') {
        mutable_args[0].erase(0, 1);
        // This search is a regex...
        for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
          for (std::map<unsigned long,Item*>::iterator o_it = (*a_it)->items().begin(); o_it != (*a_it)->items().end(); ++o_it) {
            if (o_it->second->identifiers().matchesKeyword(mutable_args[0])) {
              items.push_back(o_it->second);
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
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
    for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
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

  for (std::set<Zone*,zone_comp>::iterator a_it = World::Instance().getZones().begin(); a_it != World::Instance().getZones().end(); ++a_it) {
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
      being->room()->send_cond("$a has created $o.", being, item);
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
    avatar()->room()->send_cond("$a fades into existence.", avatar());
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

CmdJobs::CmdJobs(void) {
  name("jobs");
  level(Being::CREATOR);
  addSyntax(0, "");
  addSyntax(1, "<limit> (default is 15)");
  brief("Display the top <limit> jobs in the global schedule.");
  return;
}

bool CmdJobs::execute(Being* being, const std::vector<std::string>& args) {
  int x     = 0;
  int limit = 0;
  time_t now = time(NULL);

  if (args[0].empty()) {
    limit = 20;
  } else {
    limit = estring(args[0]);
    limit = Math::bound(limit, 1, 100);
  }

  for (auto iter : World::Instance().schedule()->queue()) {
    if (++x >= limit) break;
    being->send(" %3u {y%s{x in %s\n", x, iter->name().c_str(), os::realtime(iter->when() - now).c_str());
  }

  return true;
}

CmdKill::CmdKill(void) {
  name("kill");
  addSyntax(1, "<target>");
  brief("Attack a player or npc, initiating combat.");
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
    being->room()->send_cond("$a attacks $c!\n", being, target, NULL, Room::TO_NOTVICT);
    being->add_opponent(target);
  }
  return true;
}
