
#include "area.h"
#include "avatar.h"
#include "commands-aedit.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "flagTable.h"
#include "io-handler.h"
#include "room.h"
#include "terrainTable.h"
#include "world.h"

ACmdDelete::ACmdDelete(void) {
  name("delete");
  level(Creature::GOD);
  addSyntax(1, "delete");
  brief("Deletes the area.  This is NOT reversible.");
  return;
}

bool ACmdDelete::execute(Creature* creature, const std::vector<std::string>& args) {
  Area* area = avatar()->aedit();
  CmdExit exit;
  std::vector<std::string> exit_args(1);

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }

  exit.avatar(avatar());
  exit.execute(creature, exit_args);
  avatar()->send("\nYou've deleted Area %lu.\n", area->ID());
  World::Instance().bigBrother(creature, ADMIN_BIGBRO_CHANGES, "%s has deleted area %lu (%s).", avatar()->identifiers().shortname().c_str(), area->ID(), area->name().c_str());
  area->destroy();

  return true;
}

ACmdInformation::ACmdInformation(void) {
  name("information");
  level(Creature::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status of the area.");
  return;
}

bool ACmdInformation::execute(Creature* creature, const std::vector<std::string>& args) {
  Area* area = avatar()->aedit();
  std::string output;
  std::string names;
  char buffer[Socket::MAX_BUFFER];

  if (area->builders().empty()) {
    names << "{ynone{x";
  } else {
    for (auto iter : area->builders()) {
      names << "{y" << iter << "{x, ";
    }
  }

  sprintf(buffer, "AreaID: {y%lu{x\nFirst vnum: {y%lu{x\nLast vnum: {y%lu{x\nDefault terrain: {y%s{x\nPermission: %s", area->ID(), area->low(), area->high(), area->terrain()->name().c_str(), names.c_str());
  output.append(buffer);

  avatar()->send(output);
  return true;
}

ACmdName::ACmdName(void) {
  name("name");
  level(Creature::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Changes the name of the area.");
  return;
}

bool ACmdName::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)creature;
  Area* area = avatar->aedit();

  area->name(args[0].c_str());
  avatar->send("You've reset area %d's name to \"{W%s{x\"!", area->ID(), area->name().c_str());

  return true;
}

ACmdPermission::ACmdPermission(void) {
  name("permission");
  level(Creature::GOD);
  addSyntax(2, "grant <player>");
  addSyntax(2, "revoke <player>");
  brief("Updates access privileges for the area.");
  return;
}

bool ACmdPermission::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  if (!(avatar()->adminFlags().test(ADMIN_HEADBUILDER) || avatar()->level() >= Creature::CREATOR)) {
    avatar()->send("Sorry, only the Head Builder can fiddle with permissions.");
    return false;
  }

  if (args[0] == "grant") {
    if ((target = World::Instance().findAvatar(args[1])) == NULL) {
      avatar()->send("They couldn't be found.");
      return false;
    }
    if (target->level() < Creature::DEMIGOD) {
      avatar()->send("%s isn't a builder.", target->name());
      return false;
    }
    if (avatar()->aedit()->hasPermission(target)) {
      avatar()->send("%s already has permission to %s.", target->name(), avatar()->aedit()->name().c_str());
      return false;
    }
    avatar()->aedit()->grantPermission(target);
    avatar()->send("You've {Ggranted{x %s access to %s.", target->name(), avatar()->aedit()->name().c_str());
    target->send("%s has {Ggranted{x you access to %s.", target->seeName(creature, true).c_str(), avatar()->aedit()->name().c_str());
  } else if (args[0] == "revoke") {
    if ((target = World::Instance().findAvatar(args[1])) == NULL) {
      avatar()->send("They couldn't be found.");
      return false;
    }
    if (!avatar()->aedit()->hasPermission(target)) {
      avatar()->send("%s doesn't have permission to %s.", target->name(), avatar()->aedit()->name().c_str());
      return false;
    }
    avatar()->aedit()->revokePermission(target);
    avatar()->send("You've {rrevoked{x %s's access to %s.", target->name(), avatar()->aedit()->name().c_str());
    target->send("%s has {rrevoked{x your access to %s.", target->seeName(creature, true).c_str(), avatar()->aedit()->name().c_str());
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  return true;
}

ACmdTerrain::ACmdTerrain(void) {
  name("terrain");
  level(Creature::DEMIGOD);
  addSyntax(1, "<string>");
  brief("Changes the default terrain type of the area.");
  addOptions("terrain", std::string("\n").append(TerrainTable::Instance().list()));
  return;
}

bool ACmdTerrain::execute(Creature* creature, const std::vector<std::string>& args) {
  Terrain* t = TerrainTable::Instance().find(args[0]);
  if (t) {
    avatar()->aedit()->terrain(t);
    avatar()->send("Area default terrain set to {%c%s{x.", avatar()->aedit()->terrain()->title(), avatar()->aedit()->terrain()->name().c_str());
    return true;
  } else {
    avatar()->send("Sorry, '%s{x' isn't a terrain type.", args[0].c_str());
    return false;
  }
}
