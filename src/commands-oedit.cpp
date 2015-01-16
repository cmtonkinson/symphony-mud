/*
 * $Id: commands-oedit.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "commands-oedit.h"
#include "commandTable.h"
#include "commandTable-default.h"
#include "compoundTable.h"
#include "display.h"
#include "flagTable.h"
#include "io-handler.h"
#include "object-furniture.h"
#include "object-weapon.h"
#include "room.h"
#include "world.h"

/*
OCmd::OCmd(void) {
  name("");
  level(DEMIGOD);
  addSyntax(0, "");
  brief("");
  return;
}

bool OCmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

OCmdAttribute::OCmdAttribute(void) {
  name("attribute");
  level(DEMIGOD);
  addSyntax(2, "remove <attribute>");
  addSyntax(3, "add <attribute> <number>");
  brief("Updates the attribute modifiers of the Object.");
  addOptions("attribute", Creature::listAttributes());
  return;
}

bool OCmdAttribute::execute(Creature* creature, const std::vector<std::string>& args) {
  unsigned short attr = 0;
  int mag = 0;

  if (args.size() == 3 && Regex::strPrefix(args[0], "add")) {
    if ((attr = Creature::stringToAttribute(args[1])) == ATTR_END) {
      avatar()->send("Sorry, that attribute wasn't recognized.");
      return false;
    }
    mag = estring(args[2]);
    if (!mag) {
      avatar()->send("That wouldn't do anything.");
      return false;
    }
    if (attr == ATTR_ARMOR || attr == ATTR_BASH || attr == ATTR_SLASH || attr == ATTR_PIERCE || attr == ATTR_EXOTIC) {
      if (!avatar()->oedit()->isArmor()) {
        avatar()->send("Only objects of type `armor` can modify armor class.");
        return false;
      }
    }
    avatar()->oedit()->modifiers().push_back(new Modifier(attr, mag));
    avatar()->send("You've added a modification of %d to %s.", mag, Creature::attributeToString(attr));
  } else if (args.size() == 2 && Regex::strPrefix(args[0], "remove")) {
    if ((attr = Creature::stringToAttribute(args[1])) == ATTR_END) {
      avatar()->send("Sorry, that attribute wasn't recognized.");
      return false;
    }
    for (std::list<Modifier*>::iterator it = avatar()->oedit()->modifiers().begin(); it != avatar()->oedit()->modifiers().end(); ++it) {
      if ((*it)->attribute() == attr) {
        avatar()->oedit()->modifiers().erase(it);
        avatar()->send("You've dropped the %s modification.", Creature::attributeToString(attr));
        return true;
      }
    }
    avatar()->send("There aren't any %s modifications currently set.", Creature::attributeToString(attr));
    return false;
  } else {
    avatar()->send(printSyntax());
    return false;
  }

  return true;
}

OCmdComposition::OCmdComposition(void) {
  name("composition");
  level(DEMIGOD);
  addSyntax(-2, "add <type1 type2 type3 ...>");
  addSyntax(-2, "remove <type1 type2 type3 ...>");
  brief("Updates the composition of the Object.");
  addOptions("type", std::string("\n").append(CompoundTable::Instance().list()));
  return;
}

bool OCmdComposition::execute(Creature* creature, const std::vector<std::string>& args) {
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
        avatar()->oedit()->composition().insert(compound);
      } else {
        avatar()->oedit()->composition().erase(compound);
      }
    }
  }

  avatar()->send("Object composition is now: %s", avatar()->oedit()->implodeComposition().c_str());
  return true;
}

OCmdDelete::OCmdDelete(void) {
  name("delete");
  level(DEMIGOD);
  addSyntax(1, "delete");
  brief("Erases the Object entirely.");
  return;
}

bool OCmdDelete::execute(Creature* creature, const std::vector<std::string>& args) {
  CmdExit exit;
  std::vector<std::string> exit_args(1);
  Area* area = NULL;
  Object* object = avatar()->oedit();
  unsigned long vnum = object->vnum();

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }
  if ((area = World::Instance().lookup(vnum)) == NULL) {
    avatar()->send("Error establishing vnum-area association.");
    return false;
  }

  area->objects().erase(vnum);
  object->destroy(World::Instance().getMysql());
  avatar()->oedit(NULL);
  avatar()->send("Object %lu deleted.\n", vnum);
  exit.avatar(avatar());
  exit.execute(creature, exit_args);
  return true;
}

OCmdDescription::OCmdDescription(void) {
  name("description");
  addSyntax(0, "");
  brief("Invokes the Text Editor to change the Objects description.");
  return;
}

bool OCmdDescription::execute(Creature* creature, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(creature);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->oedit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Object Description"));
  h->getState()["pointer"] = (void*)avatar()->oedit()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

OCmdFlag::OCmdFlag(void) {
  name("flag");
  level(DEMIGOD);
  addSyntax(-2, "add <flag1 flag2 flag3 ...>");
  addSyntax(-2, "remove <flag1 flag2 flag3 ...>");
  brief("Updates which flags are set for the Object.");
  addOptions("flag", FTObject::Instance().dump());
  return;
}

bool OCmdFlag::execute(Creature* creature, const std::vector<std::string>& args) {
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
    if ((f = FTObject::Instance().search(*it)) > 0) {
      avatar()->oedit()->flags().set(f, value);
      avatar()->send("%s the %s flag.\n", output, FTObject::Instance().find(f));
    }
  }
  avatar()->send("Object flags are now: %s", avatar()->oedit()->flags().list(FTObject::Instance()).c_str());
  free(output);
  return true;
}

OCmdFurnitureCapacity::OCmdFurnitureCapacity(void) {
  name("furn_capacity");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the total overal capacity of the furniture.");
  return;
}

bool OCmdFurnitureCapacity::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* furniture = avatar()->oedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Object %lu isn't furniture.", furniture->vnum());
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

OCmdFurnitureLayOn::OCmdFurnitureLayOn(void) {
  name("furn_layon");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of creatures that can 'lay on' this furniture.");
  return;
}

bool OCmdFurnitureLayOn::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* furniture = avatar()->oedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Object %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->layOn(number);
  avatar()->send("You've set the maximum `lay on` to %u.", furniture->furniture()->layOn());
  if (furniture->furniture()->layOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `lay on` is set higher than the overall capacity.");
  }
  return true;
}

OCmdFurnitureSitAt::OCmdFurnitureSitAt(void) {
  name("furn_sitat");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of creatures that can 'sit at' this furniture.");
  return;
}

bool OCmdFurnitureSitAt::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* furniture = avatar()->oedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Object %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->sitAt(number);
  avatar()->send("You've set the maximum `sit at` to %u.", furniture->furniture()->sitAt());
  if (furniture->furniture()->sitAt() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit at` is set higher than the overall capacity.");
  }
  return true;
}

OCmdFurnitureSitOn::OCmdFurnitureSitOn(void) {
  name("furn_siton");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of creatures that can 'sit on' this furniture.");
  return;
}

bool OCmdFurnitureSitOn::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* furniture = avatar()->oedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Object %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->sitOn(number);
  avatar()->send("You've set the maximum `sit on` to %u.", furniture->furniture()->sitOn());
  if (furniture->furniture()->sitOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `sit on` is set higher than the overall capacity.");
  }
  return true;
}

OCmdFurnitureStandOn::OCmdFurnitureStandOn(void) {
  name("furn_standon");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the number of creatures that can 'stand on' this furniture.");
  return;
}

bool OCmdFurnitureStandOn::execute(Creature* creature, const std::vector<std::string>& args) {
  Object* furniture = avatar()->oedit();
  unsigned number = estring(args[0]);

  if (!furniture->isFurniture()) {
    avatar()->send("Object %lu isn't furniture.", furniture->vnum());
    return false;
  }
  furniture->furniture()->standOn(number);
  avatar()->send("You've set the maximum `stand on` to %u.", furniture->furniture()->standOn());
  if (furniture->furniture()->standOn() > furniture->furniture()->capacity()) {
    avatar()->send("\nWarning: `stand on` is set higher than the overall capacity.");
  }
  return true;
}

OCmdInformation::OCmdInformation(void) {
  name("information");
  level(DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status of the Object.");
  return;
}

bool OCmdInformation::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->send(avatar()->oedit()->printStatus());
  return true;
}

OCmdKeywords::OCmdKeywords(void) {
  name("keywords");
  level(DEMIGOD);
  addSyntax(-1, "<key1 key2 key3 ...>");
  brief("Updates the Object keyword list.");
  return;
}

bool OCmdKeywords::execute(Creature* creature, const std::vector<std::string>& args) {
  std::vector<std::string> keywords = Regex::explode(" ", args[0]);
  avatar()->oedit()->identifiers().getKeywords().clear();
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    avatar()->oedit()->identifiers().addKeyword(*it);
  }
  avatar()->send("You've set the object keywords to \"%s\".", avatar()->oedit()->identifiers().getKeywordList().c_str());
  return true;
}

OCmdLevel::OCmdLevel(void) {
  name("level");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the Object level.");
  return;
}

bool OCmdLevel::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->level(estring(args[0]));
  avatar()->send("You've set the object level to %u.", avatar()->oedit()->level());
  return true;
}

OCmdLongname::OCmdLongname(void) {
  name("longname");
  level(DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Objects long name.");
  return;
}

bool OCmdLongname::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->identifiers().longname(args[0]);
  avatar()->send("You've set the object longname to \"%s{x\".", avatar()->oedit()->identifiers().longname().c_str());

  return true;
}

OCmdShortname::OCmdShortname(void) {
  name("shortname");
  level(DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Objects short name.");
  return;
}

bool OCmdShortname::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->identifiers().shortname(args[0]);
  avatar()->send("You've set the object shortname to \"%s{x\".", avatar()->oedit()->identifiers().shortname().c_str());
  return true;
}

OCmdType::OCmdType(void) {
  name("type");
  level(DEMIGOD);
  addSyntax(1, "<value>");
  brief("Updates the type of the Object.");
  addOptions("type", std::string("armor clothing container food furniture jewelry key trash weapon"));
  return;
}

bool OCmdType::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->stringToType(args[0]);
  avatar()->send("Object %lu is now \"%s\".\n", avatar()->oedit()->vnum(), avatar()->oedit()->typeToString());
  return true;
}

OCmdValue::OCmdValue(void) {
  name("value");
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the worth (in silver) of the Object.");
  return;
}

bool OCmdValue::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->value(estring(args[0]));
  avatar()->send("You've set the object value to %u.", avatar()->oedit()->value());
  return true;
}

OCmdWeaponDamage::OCmdWeaponDamage(void) {
  name("weap_damage");
  level(DEMIGOD);
  addSyntax(1, "XdY            (X and Y are numbers)");
  brief("Changes the damage dice of the weapon.");
  return;
}

bool OCmdWeaponDamage::execute(Creature* creature, const std::vector<std::string>& args) {
  std::vector<std::string> pieces = Regex::explode("d", args[0]);
  Object* weapon = avatar()->oedit();
  unsigned number_of_dice = 0;
  unsigned faces_per_die = 0;

  if (!weapon->isWeapon()) {
    avatar()->send("Object %d isn't a weapon.", weapon->vnum());
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

OCmdWeaponType::OCmdWeaponType(void) {
  name("weap_type");
  level(DEMIGOD);
  addSyntax(1, "<type>");
  brief("Changes the kind of weapon the Object is.");
  addOptions("type", ETWeaponType::Instance().list());
  return;
}

bool OCmdWeaponType::execute(Creature* creature, const std::vector<std::string>& args) {
  if (!avatar()->oedit()->isWeapon()) {
    avatar()->send("Object %ld isn't a weapon.", avatar()->oedit()->vnum());
    return false;
  }
  avatar()->oedit()->weapon()->type().set(args[0]);
  avatar()->send("Weapon type set to %s.\n", avatar()->oedit()->weapon()->type().string().c_str());
  return true;
}

OCmdWeaponVerb::OCmdWeaponVerb(void) {
  name("weap_verb");
  level(DEMIGOD);
  addSyntax(1, "<verb>");
  brief("Changes the type of damange inflicted by the weapon.");
  addOptions("verb", ETDamageVerb::Instance().list());
  return;
}

bool OCmdWeaponVerb::execute(Creature* creature, const std::vector<std::string>& args) {
  if (!avatar()->oedit()->isWeapon()) {
    avatar()->send("Object %ld isn't a weapon.", avatar()->oedit()->vnum());
    return false;
  }
  avatar()->oedit()->weapon()->verb().set(args[0]);
  avatar()->send("Weapon verb set to %s.\n", avatar()->oedit()->weapon()->verb().string().c_str());
  return true;
}

OCmdWearable::OCmdWearable(void) {
  name("wearable");
  level(DEMIGOD);
  addSyntax(1, "<where>");
  brief("Changes where the Object can be worn (if it can be).");
  addOptions("where", "ankle arms ear face feet finger forearm hands head hold knee legs neck shin shoulders torso waist wrist");
  return;
}

bool OCmdWearable::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->oedit()->stringToWearable(args[0]);
  avatar()->send("Object can now be worn: {y%s{x", avatar()->oedit()->wearableToString());
  return true;
}
