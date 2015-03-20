
#include "zone.h"
#include "avatar.h"
#include "commands-iedit.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "compoundTable.h"
#include "display.h"
#include "flagTable.h"
#include "io-handler.h"
#include "item-furniture.h"
#include "item-weapon.h"
#include "room.h"
#include "world.h"

ICmdAttribute::ICmdAttribute(void) {
  name("attribute");
  level(Being::DEMIGOD);
  addSyntax(2, "remove <attribute>");
  addSyntax(3, "add <attribute> <number>");
  brief("Updates the attribute modifiers of the Item.");
  addOptions("attribute", Being::listAttributes());
  return;
}

bool ICmdAttribute::execute(Being* being, const std::vector<std::string>& args) {
  unsigned short attr = 0;
  int mag = 0;

  if (args.size() == 3 && Regex::strPrefix(args[0], "add")) {
    if ((attr = Being::stringToAttribute(args[1])) == Being::ATTR_END) {
      avatar()->send("Sorry, that attribute wasn't recognized.");
      return false;
    }
    mag = estring(args[2]);
    if (!mag) {
      avatar()->send("That wouldn't do anything.");
      return false;
    }
    if (attr == Being::ATTR_ARMOR || attr == Being::ATTR_BASH || attr == Being::ATTR_SLASH || attr == Being::ATTR_PIERCE || attr == Being::ATTR_EXOTIC) {
      if (!avatar()->iedit()->isArmor()) {
        avatar()->send("Only items of type `armor` can modify armor class.");
        return false;
      }
    }
    avatar()->iedit()->modifiers().push_back(new Modifier(attr, mag));
    avatar()->send("You've added a modification of %d to %s.", mag, Being::attributeToString(attr));
  } else if (args.size() == 2 && Regex::strPrefix(args[0], "remove")) {
    if ((attr = Being::stringToAttribute(args[1])) == Being::ATTR_END) {
      avatar()->send("Sorry, that attribute wasn't recognized.");
      return false;
    }
    for (std::list<Modifier*>::iterator it = avatar()->iedit()->modifiers().begin(); it != avatar()->iedit()->modifiers().end(); ++it) {
      if ((*it)->attribute() == attr) {
        avatar()->iedit()->modifiers().erase(it);
        avatar()->send("You've dropped the %s modification.", Being::attributeToString(attr));
        return true;
      }
    }
    avatar()->send("There aren't any %s modifications currently set.", Being::attributeToString(attr));
    return false;
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  return true;
}

ICmdComposition::ICmdComposition(void) {
  name("composition");
  level(Being::DEMIGOD);
  addSyntax(-2, "add <type1 type2 type3 ...>");
  addSyntax(-2, "remove <type1 type2 type3 ...>");
  brief("Updates the composition of the Item.");
  addOptions("type", std::string("\n").append(CompoundTable::Instance().list()));
  return;
}

bool ICmdComposition::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string>comps;
  Compound* compound = NULL;
  bool add = false;

  comps = Regex::explode(" ", args[1], true);
  if (Regex::strPrefix(args[0], "add")) {
    add = true;
  } else if (Regex::strPrefix(args[0], "remove")) {
    add = false;
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  for (std::vector<std::string>::iterator it = comps.begin(); it != comps.end(); ++it) {
    if ((compound = CompoundTable::Instance().find(*it)) != NULL) {
      if (add) {
        avatar()->iedit()->composition().insert(compound);
      } else {
        avatar()->iedit()->composition().erase(compound);
      }
    }
  }

  avatar()->send("Item composition is now: %s", avatar()->iedit()->serializeComposition(" ").c_str());
  return true;
}

ICmdDelete::ICmdDelete(void) {
  name("delete");
  level(Being::DEMIGOD);
  addSyntax(1, "delete");
  brief("Erases the Item entirely.");
  return;
}

bool ICmdDelete::execute(Being* being, const std::vector<std::string>& args) {
  CmdExit exit;
  std::vector<std::string> exit_args(1);
  Zone* zone = NULL;
  Item* item = avatar()->iedit();
  unsigned long vnum = item->vnum();

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }
  if ((zone = World::Instance().lookup(vnum)) == NULL) {
    avatar()->send("Error establishing vnum-zone association.");
    return false;
  }

  zone->items().erase(vnum);
  avatar()->iedit(NULL);
  avatar()->send("Item %lu deleted.\n", vnum);
  exit.avatar(avatar());
  exit.execute(being, exit_args);
  return true;
}

ICmdDescription::ICmdDescription(void) {
  name("description");
  addSyntax(0, "");
  brief("Invokes the Text Editor to change the Items description.");
  return;
}

bool ICmdDescription::execute(Being* being, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(being);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->iedit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Item Description"));
  h->getState()["pointer"] = (void*)avatar()->iedit()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

ICmdFlag::ICmdFlag(void) {
  name("flag");
  level(Being::DEMIGOD);
  addSyntax(-2, "add <flag1 flag2 flag3 ...>");
  addSyntax(-2, "remove <flag1 flag2 flag3 ...>");
  brief("Updates which flags are set for the Item.");
  addOptions("flag", FTItem::Instance().dump());
  return;
}

bool ICmdFlag::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> mutable_args = args;
  std::vector<std::string> flags;
  FlagInt f = 0;
  bool value = false;
  char* output = NULL;

  if (args.size() != 2) {
    avatar()->send(printSyntax());
    return false;
  }

  flags = Regex::explode(" ", args[1], true);
  if (Regex::strPrefix(mutable_args[0], "add")) {
    value = true;
    output = strdup("Added");
  } else if (Regex::strPrefix(mutable_args[0], "remove")) {
    value = false;
    output = strdup("Removed");
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  for (std::vector<std::string>::iterator it = flags.begin(); it != flags.end(); ++it) {
    if ((f = FTItem::Instance().search(*it)) > 0) {
      avatar()->iedit()->flags().set(f, value);
      avatar()->send("%s the %s flag.\n", output, FTItem::Instance().find(f));
    }
  }
  avatar()->send("Item flags are now: %s", avatar()->iedit()->flags().list(FTItem::Instance()).c_str());
  free(output);
  return true;
}

ICmdFurnitureCapacity::ICmdFurnitureCapacity(void) {
  name("furn_capacity");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the total overal capacity of the furniture.");
  return;
}

bool ICmdFurnitureCapacity::execute(Being* being, const std::vector<std::string>& args) {
  Item* furniture = avatar()->iedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Item %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->capacity(number);
  avatar()->send("You've set the total furniture capacity to %u.", furniture->furniture()->capacity());
  if (furniture->furniture()->layOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `lay on` is set higher than total capacity.");
  }
  if (furniture->furniture()->sitAt() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit at` is set higher than total capacity.");
  }
  if (furniture->furniture()->sitOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit on` is set higher than total capacity.");
  }
  if (furniture->furniture()->standOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `stand on` is set higher than total capacity.");
  }
  return true;
}

ICmdFurnitureLayOn::ICmdFurnitureLayOn(void) {
  name("furn_layon");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of beings that can 'lay on' this furniture.");
  return;
}

bool ICmdFurnitureLayOn::execute(Being* being, const std::vector<std::string>& args) {
  Item* furniture = avatar()->iedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Item %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->layOn(number);
  avatar()->send("You've set the maximum `lay on` to %u.", furniture->furniture()->layOn());
  if (furniture->furniture()->layOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `lay on` is set higher than the overall capacity.");
  }
  return true;
}

ICmdFurnitureSitAt::ICmdFurnitureSitAt(void) {
  name("furn_sitat");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of beings that can 'sit at' this furniture.");
  return;
}

bool ICmdFurnitureSitAt::execute(Being* being, const std::vector<std::string>& args) {
  Item* furniture = avatar()->iedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Item %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->sitAt(number);
  avatar()->send("You've set the maximum `sit at` to %u.", furniture->furniture()->sitAt());
  if (furniture->furniture()->sitAt() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit at` is set higher than the overall capacity.");
  }
  return true;
}

ICmdFurnitureSitOn::ICmdFurnitureSitOn(void) {
  name("furn_siton");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of beings that can 'sit on' this furniture.");
  return;
}

bool ICmdFurnitureSitOn::execute(Being* being, const std::vector<std::string>& args) {
  Item* furniture = avatar()->iedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Item %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->sitOn(number);
  avatar()->send("You've set the maximum `sit on` to %u.", furniture->furniture()->sitOn());
  if (furniture->furniture()->sitOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit on` is set higher than the overall capacity.");
  }
  return true;
}

ICmdFurnitureStandOn::ICmdFurnitureStandOn(void) {
  name("furn_standon");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of beings that can 'stand on' this furniture.");
  return;
}

bool ICmdFurnitureStandOn::execute(Being* being, const std::vector<std::string>& args) {
  Item* furniture = avatar()->iedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Item %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->standOn(number);
  avatar()->send("You've set the maximum `stand on` to %u.", furniture->furniture()->standOn());
  if (furniture->furniture()->standOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `stand on` is set higher than the overall capacity.");
  }
  return true;
}

ICmdInformation::ICmdInformation(void) {
  name("information");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status of the Item.");
  return;
}

bool ICmdInformation::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->send(avatar()->iedit()->printStatus());
  return true;
}

ICmdKeywords::ICmdKeywords(void) {
  name("keywords");
  level(Being::DEMIGOD);
  addSyntax(-1, "<key1 key2 key3 ...>");
  brief("Updates the Item keyword list.");
  return;
}

bool ICmdKeywords::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> keywords = Regex::explode(" ", args[0]);
  avatar()->iedit()->identifiers().getKeywords().clear();
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    avatar()->iedit()->identifiers().addKeyword(*it);
  }
  avatar()->send("You've set the item keywords to \"%s\".", avatar()->iedit()->identifiers().getKeywordList().c_str());
  return true;
}

ICmdLevel::ICmdLevel(void) {
  name("level");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the Item level.");
  return;
}

bool ICmdLevel::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->level(estring(args[0]));
  avatar()->send("You've set the item level to %u.", avatar()->iedit()->level());
  return true;
}

ICmdLongname::ICmdLongname(void) {
  name("longname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Items long name.");
  return;
}

bool ICmdLongname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->identifiers().longname(args[0]);
  avatar()->send("You've set the item longname to \"%s{x\".", avatar()->iedit()->identifiers().longname().c_str());

  return true;
}

ICmdShortname::ICmdShortname(void) {
  name("shortname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Items short name.");
  return;
}

bool ICmdShortname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->identifiers().shortname(args[0]);
  avatar()->send("You've set the item shortname to \"%s{x\".", avatar()->iedit()->identifiers().shortname().c_str());
  return true;
}

ICmdType::ICmdType(void) {
  name("type");
  level(Being::DEMIGOD);
  addSyntax(1, "<value>");
  brief("Updates the type of the Item.");
  addOptions("type", std::string("armor clothing container food furniture jewelry key trash weapon"));
  return;
}

bool ICmdType::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->stringToType(args[0]);
  avatar()->send("Item %lu is now \"%s\".\n", avatar()->iedit()->vnum(), avatar()->iedit()->typeToString());
  return true;
}

ICmdValue::ICmdValue(void) {
  name("value");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the worth (in silver) of the Item.");
  return;
}

bool ICmdValue::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->value(estring(args[0]));
  avatar()->send("You've set the item value to %u.", avatar()->iedit()->value());
  return true;
}

ICmdWeaponDamage::ICmdWeaponDamage(void) {
  name("weap_damage");
  level(Being::DEMIGOD);
  addSyntax(1, "XdY            (X and Y are numbers)");
  brief("Changes the damage dice of the weapon.");
  return;
}

bool ICmdWeaponDamage::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> pieces = Regex::explode("d", args[0]);
  Item* weapon = avatar()->iedit();
  unsigned number_of_dice = 0;
  unsigned faces_per_die = 0;

  if (!weapon->isWeapon()) {
    avatar()->send("Item %d isn't a weapon.", weapon->vnum());
    return false;
  }
  if (pieces.size() != 2) {
    avatar()->send(printSyntax());
    return false;
  }

  number_of_dice = estring(pieces[0]);
  faces_per_die = estring(pieces[1]);

  if (number_of_dice < 1 || number_of_dice > 20) {
    avatar()->send("The number of dice must be between 1 and 20, inclusive.");
    return false;
  } else if (faces_per_die < 1 || faces_per_die > 20) {
    avatar()->send("The number of faces per die must be between 1 and 20, inclusive.");
    return false;
  }

  weapon->weapon()->damage().number(number_of_dice);
  weapon->weapon()->damage().faces(faces_per_die);
  avatar()->send("You've changed the weapons damage dice to %s.", weapon->weapon()->damage().toString());
  return true;
}

ICmdWeaponType::ICmdWeaponType(void) {
  name("weap_type");
  level(Being::DEMIGOD);
  addSyntax(1, "<type>");
  brief("Changes the kind of weapon the Item is.");
  addOptions("type", ETWeaponType::Instance().list());
  return;
}

bool ICmdWeaponType::execute(Being* being, const std::vector<std::string>& args) {
  if (!avatar()->iedit()->isWeapon()) {
    avatar()->send("Item %ld isn't a weapon.", avatar()->iedit()->vnum());
    return false;
  }
  avatar()->iedit()->weapon()->type().set(args[0]);
  avatar()->send("Weapon type set to %s.\n", avatar()->iedit()->weapon()->type().string().c_str());
  return true;
}

ICmdWeaponVerb::ICmdWeaponVerb(void) {
  name("weap_verb");
  level(Being::DEMIGOD);
  addSyntax(1, "<verb>");
  brief("Changes the type of damange inflicted by the weapon.");
  addOptions("verb", ETDamageVerb::Instance().list());
  return;
}

bool ICmdWeaponVerb::execute(Being* being, const std::vector<std::string>& args) {
  if (!avatar()->iedit()->isWeapon()) {
    avatar()->send("Item %ld isn't a weapon.", avatar()->iedit()->vnum());
    return false;
  }
  avatar()->iedit()->weapon()->verb().set(args[0]);
  avatar()->send("Weapon verb set to %s.\n", avatar()->iedit()->weapon()->verb().string().c_str());
  return true;
}

ICmdWearable::ICmdWearable(void) {
  name("wearable");
  level(Being::DEMIGOD);
  addSyntax(1, "<where>");
  brief("Changes where the Item can be worn (if it can be).");
  addOptions("where", "ankle arms ear face feet finger forearm hands head hold knee legs neck shin shoulders torso waist wrist");
  return;
}

bool ICmdWearable::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->iedit()->stringToWearable(args[0]);
  avatar()->send("Item can now be worn: {y%s{x", avatar()->iedit()->wearableToString());
  return true;
}
