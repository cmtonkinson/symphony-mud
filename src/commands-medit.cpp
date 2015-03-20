
#include "commands-medit.h"
#include "being.h"
#include "display.h"
#include "io-handler.h"
#include "mob.h"
#include "room.h"

MCmdAggressiveness::MCmdAggressiveness(void) {
  name("aggressivenes");
  level(Being::DEMIGOD);
  addSyntax(1, "<aggressivenes>");
  brief("Change the aggressivenes type of the Mob.");
  addOptions("aggressivenes", "Five point scale:\n  1 - scared\n  2 - passive\n  3 - neutral (default)\n  4 - aggressive\n  5 - mean");
  return;
}

bool MCmdAggressiveness::execute(Being* being, const std::vector<std::string>& args) {
  unsigned aggressiveness = estring(args[0]);
  if (aggressiveness < Mob::MIN_AGGRESSIVENESS || aggressiveness > Mob::MAX_AGGRESSIVENESS) {
    being->send("Invalid aggressivenes.");
    being->send(printSyntax());
    return false;
  }
  avatar()->medit()->aggressiveness(aggressiveness);
  avatar()->send("This Mob is now a %u on the aggressiveness scale.\n", avatar()->medit()->aggressiveness());
  return true;
}

MCmdClass::MCmdClass(void) {
  name("class");
  level(Being::DEMIGOD);
  addSyntax(1, "<class>");
  brief("Change the class type of the Mob.");
  addOptions("class", ETPClass::Instance().list());
  return;
}

bool MCmdClass::execute(Being* being, const std::vector<std::string>& args) {
  int pclass = 0;
  if ((pclass = ETPClass::Instance().get(args[0])) == 0) {
    being->send("Invalid class.");
    return false;
  }
  avatar()->medit()->pClass().set(pclass);
  avatar()->send("This Mob is now a %s.\n", avatar()->medit()->pClass().string().c_str());
  return true;
}

MCmdDescription::MCmdDescription(void) {
  name("description");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Invokes the Text Editor for the Mobs' description.");
  return;
}

bool MCmdDescription::execute(Being* being, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(being);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->medit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Mob Description"));
  h->getState()["pointer"] = (void*)avatar()->medit()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

MCmdGender::MCmdGender(void) {
  name("gender");
  level(Being::DEMIGOD);
  addSyntax(1, "<gender>");
  brief("Change the gender of the Mob.");
  addOptions("gender", ETGender::Instance().list());
  return;
}

bool MCmdGender::execute(Being* being, const std::vector<std::string>& args) {
  int gender = 0;
  if ((gender = ETGender::Instance().get(args[0])) == 0) {
    being->send("Invalid gender.");
    return false;
  }
  avatar()->medit()->gender().set(gender);
  avatar()->send("This Mob is now %s.\n", avatar()->medit()->gender().string().c_str());
  return true;
}

MCmdInformation::MCmdInformation(void) {
  name("information");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays information about the Mob.");
  return;
}

bool MCmdInformation::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->send(Mob::getInformation(avatar()->medit()));
  return true;
}

MCmdKeywords::MCmdKeywords(void) {
  name("keywords");
  level(Being::DEMIGOD);
  addSyntax(-1, "<key1 key2 key3 ...>");
  brief("Updtes the Mobs keywords.");
  return;
}

bool MCmdKeywords::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> keywords = Regex::explode(" ", args[0]);
  avatar()->medit()->identifiers().getKeywords().clear();
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    avatar()->medit()->identifiers().addKeyword(*it);
  }
  avatar()->send("You've set the mob keywords to \"%s\".", avatar()->medit()->identifiers().getKeywordList().c_str());
  return true;
}

MCmdLevel::MCmdLevel(void) {
  name("level");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the Mobs level.");
  addOptions("level", "must be between 1 and 100, inclusive");
  return;
}

bool MCmdLevel::execute(Being* being, const std::vector<std::string>& args) {
  int level = estring(args[0]);
  if (level > 0 && level < 101) {
    avatar()->medit()->level(level);
    avatar()->send("You've set the mob level to %u.", avatar()->medit()->level());
    return true;
  } else {
    avatar()->send(printSyntax());
    return false;
  }
}

MCmdLongname::MCmdLongname(void) {
  name("longname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Mobs long name.");
  return;
}

bool MCmdLongname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->medit()->identifiers().longname(args[0]);
  avatar()->send("You've set the mob longname to \"%s\".", avatar()->medit()->identifiers().longname().c_str());
  return true;
}

MCmdMobility::MCmdMobility(void) {
  name("mobility");
  level(Being::DEMIGOD);
  addSyntax(1, "<mobility>");
  brief("Change the mobility type of the Mob.");
  addOptions("mobility", "Five point scale:\n  1 - stationary\n  2 - sluggish\n  3 - normal (default)\n  4 - roving\n  5 - hyperactive");
  return;
}

bool MCmdMobility::execute(Being* being, const std::vector<std::string>& args) {
  unsigned mobility = estring(args[0]);
  if (mobility < Mob::MIN_AGGRESSIVENESS || mobility > Mob::MAX_AGGRESSIVENESS) {
    being->send("Invalid mobility.");
    being->send(printSyntax());
    return false;
  }
  avatar()->medit()->mobility(mobility);
  avatar()->send("This Mob is now a %u on the mobility scale.\n", avatar()->medit()->mobility());
  return true;
}

MCmdRace::MCmdRace(void) {
  name("race");
  level(Being::DEMIGOD);
  addSyntax(1, "<race>");
  brief("Change the race of the Mob.");
  addOptions("race", ETRace::Instance().list());
  return;
}

bool MCmdRace::execute(Being* being, const std::vector<std::string>& args) {
  int race = 0;
  if ((race = ETRace::Instance().get(args[0])) == 0) {
    being->send("Invalid race.");
    return false;
  }
  avatar()->medit()->race().set(race);
  avatar()->send("This Mob is now %s.\n", avatar()->medit()->race().string().c_str());
  return true;
}

MCmdShortname::MCmdShortname(void) {
  name("shortname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Mobs short name.");
  return;
}

bool MCmdShortname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->medit()->identifiers().shortname(args[0]);
  avatar()->send("You've set the mob shortname to \"%s\".", avatar()->medit()->identifiers().shortname().c_str());
  return true;
}
