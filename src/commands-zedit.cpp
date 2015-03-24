
#include "avatar.hpp"
#include "command-def.hpp"
#include "display.hpp"
#include "flag-table.hpp"
#include "io-handler.hpp"
#include "os.hpp"
#include "room.hpp"
#include "terrain-table.hpp"
#include "world.hpp"
#include "zone.hpp"

ZCmdDelete::ZCmdDelete(void) {
  name("delete");
  level(Being::GOD);
  addSyntax(1, "delete");
  brief("Deletes the zone.  This is NOT reversible.");
  return;
}

bool ZCmdDelete::execute(Being* being, const std::vector<std::string>& args) {
  Zone* zone = avatar()->zedit();
  CmdExit exit;
  std::vector<std::string> exit_args(1);

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }

  exit.avatar(avatar());
  exit.execute(being, exit_args);
  zone->destroy();
  avatar()->send("\nYou've deleted Zone %lu.\n", zone->ID());

  INFO_(being, "zone '%s' deleted", zone->name().c_str())
  return true;
}

ZCmdInformation::ZCmdInformation(void) {
  name("information");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status of the zone.");
  return;
}

bool ZCmdInformation::execute(Being* being, const std::vector<std::string>& args) {
  Zone* zone = avatar()->zedit();
  std::string output;
  std::string names;
  char buffer[Socket::MAX_BUFFER];

  if (zone->builders().empty()) {
    names << "{ynone{x";
  } else {
    for (auto iter : zone->builders()) {
      names << "{y" << iter << "{x, ";
    }
  }

  sprintf(buffer, "ZoneID: {y%lu{x\nFirst vnum: {y%lu{x\nLast vnum: {y%lu{x\nDefault terrain: {y%s{x\nPermission: %s", zone->ID(), zone->low(), zone->high(), zone->terrain()->name().c_str(), names.c_str());
  output.append(buffer);

  avatar()->send(output);
  return true;
}

ZCmdName::ZCmdName(void) {
  name("name");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Changes the name of the zone.");
  return;
}

bool ZCmdName::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)being;
  Zone* zone = avatar->zedit();

  zone->name(args[0].c_str());
  avatar->send("You've reset zone %d's name to \"{W%s{x\"!", zone->ID(), zone->name().c_str());

  return true;
}

ZCmdPermission::ZCmdPermission(void) {
  name("permission");
  level(Being::GOD);
  addSyntax(2, "grant <player>");
  addSyntax(2, "revoke <player>");
  brief("Updates access privileges for the zone.");
  return;
}

bool ZCmdPermission::execute(Being* being, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  if (!(avatar()->adminFlags().test(ADMIN_HEADBUILDER) || avatar()->level() >= Being::CREATOR)) {
    avatar()->send("Sorry, only the Head Builder can fiddle with permissions.");
    return false;
  }

  if (args[0] == "grant") {
    if ((target = World::Instance().findAvatar(args[1])) == NULL) {
      avatar()->send("They couldn't be found.");
      return false;
    }
    if (target->level() < Being::DEMIGOD) {
      avatar()->send("%s isn't a builder.", target->name());
      return false;
    }
    if (avatar()->zedit()->hasPermission(target)) {
      avatar()->send("%s already has permission to %s.", target->name(), avatar()->zedit()->name().c_str());
      return false;
    }
    avatar()->zedit()->grantPermission(target);
    avatar()->send("You've {Ggranted{x %s access to %s.", target->name(), avatar()->zedit()->name().c_str());
    target->send("%s has {Ggranted{x you access to %s.", target->seeName(being, true).c_str(), avatar()->zedit()->name().c_str());
  } else if (args[0] == "revoke") {
    if ((target = World::Instance().findAvatar(args[1])) == NULL) {
      avatar()->send("They couldn't be found.");
      return false;
    }
    if (!avatar()->zedit()->hasPermission(target)) {
      avatar()->send("%s doesn't have permission to %s.", target->name(), avatar()->zedit()->name().c_str());
      return false;
    }
    avatar()->zedit()->revokePermission(target);
    avatar()->send("You've {rrevoked{x %s's access to %s.", target->name(), avatar()->zedit()->name().c_str());
    target->send("%s has {rrevoked{x your access to %s.", target->seeName(being, true).c_str(), avatar()->zedit()->name().c_str());
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  return true;
}

ZCmdTerrain::ZCmdTerrain(void) {
  name("terrain");
  level(Being::DEMIGOD);
  addSyntax(1, "<string>");
  brief("Changes the default terrain type of the zone.");
  addOptions("terrain", std::string("\n").append(TerrainTable::Instance().list()));
  return;
}

bool ZCmdTerrain::execute(Being* being, const std::vector<std::string>& args) {
  Terrain* t = TerrainTable::Instance().find(args[0]);
  if (t) {
    avatar()->zedit()->terrain(t);
    avatar()->send("Zone default terrain set to {%c%s{x.", avatar()->zedit()->terrain()->title(), avatar()->zedit()->terrain()->name().c_str());
    return true;
  } else {
    avatar()->send("Sorry, '%s{x' isn't a terrain type.", args[0].c_str());
    return false;
  }
}
