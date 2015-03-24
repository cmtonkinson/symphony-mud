
#include "zone.hpp"
#include "avatar.hpp"
#include "command-def.hpp"
#include "display.hpp"
#include "io-handler.hpp"
#include "room.hpp"
#include "terrain-table.hpp"
#include "world.hpp"

RCmdCreate::RCmdCreate(void) {
  name("create");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  addSyntax(1, "<vnum>");
  brief("Creates a new Room in the zone.");
  return;
}

bool RCmdCreate::execute(Being* being, const std::vector<std::string>& args) {
  unsigned long vnum = 0;
  Zone* zone = NULL;
  Room* room = NULL;

  if (args[0].empty()) {
    // vnum is being implied
    vnum = avatar()->room()->zone()->lowestAvailableRoom();
    if (!vnum) {
      avatar()->send("There are no empty vnums in the current zone.");
      return false;
    }
  } else {
    // explicit vnum
    vnum = estring(args[0]);
  }

  // Does the zone exist?
  if ((zone = World::Instance().lookup(vnum)) == NULL) {
    avatar()->send("That vnum doesn't exist.");
    return false;
  }

  // Do they have permission to the vnum?
  if (!zone->hasPermission((Avatar*)being)) {
    avatar()->send("You can't access that vnum.");
    return false;
  }

  // Does the room already exist?
  if (World::Instance().findRoom(vnum) != NULL) {
    avatar()->send("That room already exists.");
    return false;
  }

  // Create the room...
  room = new Room(vnum, zone);
  avatar()->send("You've created room %lu!", room->vnum());

  return true;
}

RCmdDelete::RCmdDelete(void) {
  name("delete");
  level(Being::DEMIGOD);
  addSyntax(1, "delete");
  brief("Erases the Room.");
  return;
}

bool RCmdDelete::execute(Being* being, const std::vector<std::string>& args) {
  Zone* zone = NULL;
  Room* room = avatar()->room();
  unsigned long vnum = room->vnum();

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }
  if ((zone = World::Instance().lookup(vnum)) == NULL) {
    avatar()->send("Error establishing vnum-zone association.");
    return false;
  }

  room->destroy();
  avatar()->send("\nRoom %lu deleted.\n", vnum);
  return true;
}

RCmdDescription::RCmdDescription(void) {
  name("description");
  addSyntax(0, "");
  brief("Invokes the Text Editor to update the Room description.");
  return;
}

bool RCmdDescription::execute(Being* being, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(being);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->room()->description())));
  h->getState()["name"] = (void*)(new std::string("Room Description"));
  h->getState()["pointer"] = (void*)avatar()->room()->descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

RCmdDig::RCmdDig(void) {
  name("dig");
  level(Being::DEMIGOD);
  addSyntax(1, "<direction>");
  addSyntax(2, "<direction> <vnum>");
  brief("Create a new Room in the Zone, and automatically link with the current Room.");
  return;
}

bool RCmdDig::execute(Being* being, const std::vector<std::string>& args) {
  unsigned long target = 0;
  RCmdCreate create;
  std::vector<std::string> create_args;
  RCmdLink link;
  std::vector<std::string> link_args;
  CmdGoto go;
  std::vector<std::string> go_args;

  // Get the vnum...
  if (args.size() == 1) { // vnum is being implied
    target = avatar()->room()->zone()->lowestAvailableRoom();
  } else if (args.size() == 2) { // explicit vnum
    target = estring(args[1]);
  }

  // Get the direction...
  if (Exit::string2dir(args[0]) == UNDEFINED) {
    avatar()->send("You can't dig that way.");
    return false;
  }

  // Create the room...
  create_args.push_back(estring(target));
  create.avatar(avatar());
  if (!create.execute(being, create_args)) {
    avatar()->send("dig failed: create");
    return false;
  }
  if (World::Instance().findRoom(target) == NULL) {
    avatar()->send("dig failed: findRoom");
    return false;
  }

  // Link to the new room...
  link_args.push_back(args[0]);
  link_args.push_back(estring(target));
  link.avatar(avatar());
  if (!link.execute(being, link_args)) {
    avatar()->send("dig failed: link");
    return false;
  }

  // Goto the new room...
  go_args.push_back(estring(target));
  go.avatar(avatar());
  if (!go.execute(being, go_args)) {
    avatar()->send("dig failed: goto");
    return false;
  }

  return true;
}

RCmdExitflag::RCmdExitflag(void) {
  name("exitflag");
  level(Being::DEMIGOD);
  addSyntax(-3, "<direction> add <flag1 flag2 flag3 ...>");
  addSyntax(-3, "<direction> remove <flag1 flag2 flag3 ...>");
  brief("Modifies flags on Exits in the Room.");
  addOptions("flag", FTExit::Instance().dump());
  return;
}

bool RCmdExitflag::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> flags;
  Exit* exit = NULL;
  unsigned short direction = 0;
  bool value = false;
  FlagInt f = 0;

  flags = Regex::explode(" ", args[2], true);
  direction = Exit::string2dir(args[0]);

  if ((exit = avatar()->room()->exit(direction)) == NULL) {
    avatar()->send("There is no exit in that direction.");
    return false;
  }

  if (Regex::strPrefix(args[1], "add")) {
    value = true;
  } else if (Regex::strPrefix(args[1], "remove")) {
    value = false;
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  for (std::vector<std::string>::iterator it = flags.begin(); it != flags.end(); ++it) {
    if ((f = FTExit::Instance().search(*it)) > 0) {
      exit->flags().set(f, value);
    }
  }

  avatar()->send("%s flags are now: %s", Exit::name(direction), exit->flags().list(FTExit::Instance()).c_str());
  return true;
}

RCmdFlag::RCmdFlag(void) {
  name("flag");
  level(Being::DEMIGOD);
  addSyntax(-2, "add <flag1 flag2 flag3 ...>");
  addSyntax(-2, "remove <flag1 flag2 flag3 ...>");
  brief("Modify the list of Room flags.");
  addOptions("flag", FTRoom::Instance().dump());
  return;
}

bool RCmdFlag::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> flags;
  bool value = false;
  FlagInt f = 0;

  flags = Regex::explode(" ", args[1], true);
  if (Regex::strPrefix(args[0], "add")) {
    value = true;
  } else if (Regex::strPrefix(args[0], "remove")) {
    value = false;
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  for (std::vector<std::string>::iterator it = flags.begin(); it != flags.end(); ++it) {
    if ((f = FTRoom::Instance().search(*it)) > 0) {
      avatar()->room()->flags().set(f, value);
    }
  }
  avatar()->send("Room flags are now: %s", avatar()->room()->flags().list(FTRoom::Instance()).c_str());
  return true;
}

RCmdInformation::RCmdInformation(void) {
  name("information");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the current status of the Room.");
  return;
}

bool RCmdInformation::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->send(Room::getInformation(avatar()->room()));
  return true;
}

RCmdKey::RCmdKey(void) {
  name("key");
  level(Being::DEMIGOD);
  addSyntax(2, "<direction> <key vnum>");
  brief("Sets a door lock to work with the specified key.");
  return;
}

bool RCmdKey::execute(Being* being, const std::vector<std::string>& args) {
  Exit* exit = NULL;
  Exit* other = NULL;
  unsigned long vnum = 0;

  if ((exit = avatar()->room()->exit(args[0])) == NULL) {
    avatar()->send("There is no exit in that direction.");
    return false;
  }

  vnum = estring(args[1]);
  exit->key(vnum);
  if ((other = exit->targetRoom()->exit(Exit::inverse(exit->direction().number()))) != NULL) {
    other->key(vnum);
  }
  avatar()->send("%s exit's key set to item %lu.\n", exit->direction().string().c_str(), exit->key());
  return true;
}

RCmdLink::RCmdLink(void) {
  name("link");
  level(Being::DEMIGOD);
  addSyntax(2, "<direction> <vnum>");
  brief("Create a two-way Exit to the Room specified by <vnum>.");
  return;
}

bool RCmdLink::execute(Being* being, const std::vector<std::string>& args) {
  Room* from = NULL;
  Room* to = NULL;
  unsigned short from_dir = 0;
  unsigned short to_dir = 0;

  if (args.size() != 2) {
    avatar()->send(printSyntax());
    return false;
  }

  // Find the target room...
  if ((to = World::Instance().findRoom(estring(args[1]))) == NULL) {
    avatar()->send("That room doesn't exist.");
    return false;
  }
  from = avatar()->room();

  // Check permissions...
  if (!to->zone()->hasPermission((Avatar*)being)) {
    avatar()->send("You don't have access to that room.");
    return false;
  }
  // Make sure the given exits don't already exist...
  if ((from_dir = Exit::string2dir(args[0])) == UNDEFINED) {
    avatar()->send("You can't link in that direction.");
    return false;
  }
  to_dir = Exit::inverse(from_dir);
  if (from->exit(from_dir) != NULL) {
    avatar()->send("There's already a %s exit here.", Exit::name(from_dir));
    return false;
  }
  if (to->exit(to_dir) != NULL) {
    avatar()->send("Room %lu already has a %s exit.", to->vnum(), Exit::name(to_dir));
    return false;
  }

  // Everything looks good; create the exits...
  from->exit(from_dir, new Exit(from, to, from_dir));
  to->exit(to_dir, new Exit(to, from, to_dir));
  avatar()->send("Two-way link established.");
  return true;
}

RCmdName::RCmdName(void) {
  name("name");
  level(Being::DEMIGOD);
  addSyntax(-1, "<name>");
  brief("Resets the Room name.");
  return;
}

bool RCmdName::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->room()->name(args[0]);
  avatar()->send("You've set room %lu's name to \"{W%s{x\".", avatar()->room()->vnum(), avatar()->room()->name().c_str());
  return true;
}

RCmdSmell::RCmdSmell(void) {
  name("smell");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Changes the smells in the Room.");
  return;
}

bool RCmdSmell::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->room()->smell(args[0]);
  avatar()->send("Room smell set to: {y%s{x", avatar()->room()->smell().c_str());
  return true;
}

RCmdSound::RCmdSound(void) {
  name("sound");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Changes the sounds in the Room.");
  return;
}

bool RCmdSound::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->room()->sound(args[0]);
  avatar()->send("Room sound set to: {y%s{x", avatar()->room()->sound().c_str());
  return true;
}

RCmdTerrain::RCmdTerrain(void) {
  name("terrain");
  level(Being::DEMIGOD);
  addSyntax(1, "<terrain>");
  brief("Changes the terrain of the Room.");
  addOptions("terrain", std::string("\n").append(TerrainTable::Instance().list()));
  return;
}

bool RCmdTerrain::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->room()->terrain(TerrainTable::Instance().find(args[0]));
  avatar()->send("Room terrain set to {%c%s{x.", avatar()->room()->terrain()->title(), avatar()->room()->terrain()->name().c_str());
  return true;
}

RCmdUnlink::RCmdUnlink(void) {
  name("unlink");
  level(Being::DEMIGOD);
  addSyntax(1, "<direction>");
  brief("Erases the Exit link in the given direction.");
  return;
}

bool RCmdUnlink::execute(Being* being, const std::vector<std::string>& args) {
  unsigned short direction = Exit::string2dir(Regex::trim(args[0]));
  unsigned short inverse = Exit::inverse(direction);
  Room* other = NULL;
  Exit* exit = NULL;
  Exit* exit2 = NULL;

  if ((exit = avatar()->room()->exit(direction)) == NULL) {
    avatar()->send("There is no exit that way.");
    return false;
  }

  // Delete the exit's counterpart first, if it has one...
  other = avatar()->room()->exit(direction)->targetRoom();
  if ((exit2 = other->exit(inverse)) != NULL) {
    exit2->destroy();
    other->exit(inverse, NULL);
  }

  // Delete the exit...
  exit->destroy();
  avatar()->room()->exit(direction, NULL);
  avatar()->send("Link removed.");
  return true;
}
