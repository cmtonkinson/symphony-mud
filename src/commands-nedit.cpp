
#include "command-def.h"
#include "being.h"
#include "display.h"
#include "io-handler.h"
#include "npc.h"
#include "room.h"

NCmdAggressiveness::NCmdAggressiveness(void) {
  name("aggressivenes");
  level(Being::DEMIGOD);
  addSyntax(1, "<aggressivenes>");
  brief("Change the aggressivenes type of the Npc.");
  addOptions("aggressivenes", "Five point scale:\n  1 - scared\n  2 - passive\n  3 - neutral (default)\n  4 - aggressive\n  5 - mean");
  return;
}

bool NCmdAggressiveness::execute(Being* being, const std::vector<std::string>& args) {
  unsigned aggressiveness = estring(args[0]);
  if (aggressiveness < Npc::MIN_AGGRESSIVENESS || aggressiveness > Npc::MAX_AGGRESSIVENESS) {
    being->send("Invalid aggressivenes.");
    being->send(printSyntax());
    return false;
  }
  avatar()->nedit()->aggressiveness(aggressiveness);
  avatar()->send("This Npc is now a %u on the aggressiveness scale.\n", avatar()->nedit()->aggressiveness());
  return true;
}

NCmdClass::NCmdClass(void) {
  name("class");
  level(Being::DEMIGOD);
  addSyntax(1, "<class>");
  brief("Change the class type of the Npc.");
  addOptions("class", ETPClass::Instance().list());
  return;
}

bool NCmdClass::execute(Being* being, const std::vector<std::string>& args) {
  int pclass = 0;
  if ((pclass = ETPClass::Instance().get(args[0])) == 0) {
    being->send("Invalid class.");
    return false;
  }
  avatar()->nedit()->pClass().set(pclass);
  avatar()->send("This Npc is now a %s.\n", avatar()->nedit()->pClass().string().c_str());
  return true;
}

NCmdDescription::NCmdDescription(void) {
  name("description");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Invokes the Text Editor for the Npcs' description.");
  return;
}

bool NCmdDescription::execute(Being* being, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(being);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->nedit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Npc Description"));
  h->getState()["pointer"] = (void*)avatar()->nedit()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

NCmdGender::NCmdGender(void) {
  name("gender");
  level(Being::DEMIGOD);
  addSyntax(1, "<gender>");
  brief("Change the gender of the Npc.");
  addOptions("gender", ETGender::Instance().list());
  return;
}

bool NCmdGender::execute(Being* being, const std::vector<std::string>& args) {
  int gender = 0;
  if ((gender = ETGender::Instance().get(args[0])) == 0) {
    being->send("Invalid gender.");
    return false;
  }
  avatar()->nedit()->gender().set(gender);
  avatar()->send("This Npc is now %s.\n", avatar()->nedit()->gender().string().c_str());
  return true;
}

NCmdInformation::NCmdInformation(void) {
  name("information");
  level(Being::DEMIGOD);
  addSyntax(0, "");
  brief("Displays information about the Npc.");
  return;
}

bool NCmdInformation::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->send(Npc::getInformation(avatar()->nedit()));
  return true;
}

NCmdKeywords::NCmdKeywords(void) {
  name("keywords");
  level(Being::DEMIGOD);
  addSyntax(-1, "<key1 key2 key3 ...>");
  brief("Updtes the Npcs keywords.");
  return;
}

bool NCmdKeywords::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<std::string> keywords = Regex::explode(" ", args[0]);
  avatar()->nedit()->identifiers().getKeywords().clear();
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    avatar()->nedit()->identifiers().addKeyword(*it);
  }
  avatar()->send("You've set the npc keywords to \"%s\".", avatar()->nedit()->identifiers().getKeywordList().c_str());
  return true;
}

NCmdLevel::NCmdLevel(void) {
  name("level");
  level(Being::DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the Npcs level.");
  addOptions("level", "must be between 1 and 100, inclusive");
  return;
}

bool NCmdLevel::execute(Being* being, const std::vector<std::string>& args) {
  int level = estring(args[0]);
  if (level > 0 && level < 101) {
    avatar()->nedit()->level(level);
    avatar()->send("You've set the npc level to %u.", avatar()->nedit()->level());
    return true;
  } else {
    avatar()->send(printSyntax());
    return false;
  }
}

NCmdLongname::NCmdLongname(void) {
  name("longname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Npcs long name.");
  return;
}

bool NCmdLongname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->nedit()->identifiers().longname(args[0]);
  avatar()->send("You've set the npc longname to \"%s\".", avatar()->nedit()->identifiers().longname().c_str());
  return true;
}

NCmdMobility::NCmdMobility(void) {
  name("mobility");
  level(Being::DEMIGOD);
  addSyntax(1, "<mobility>");
  brief("Change the mobility type of the Npc.");
  addOptions("mobility", "Five point scale:\n  1 - stationary\n  2 - sluggish\n  3 - normal (default)\n  4 - roving\n  5 - hyperactive");
  return;
}

bool NCmdMobility::execute(Being* being, const std::vector<std::string>& args) {
  unsigned mobility = estring(args[0]);
  if (mobility < Npc::MIN_AGGRESSIVENESS || mobility > Npc::MAX_AGGRESSIVENESS) {
    being->send("Invalid mobility.");
    being->send(printSyntax());
    return false;
  }
  avatar()->nedit()->mobility(mobility);
  avatar()->send("This Npc is now a %u on the mobility scale.\n", avatar()->nedit()->mobility());
  return true;
}

NCmdRace::NCmdRace(void) {
  name("race");
  level(Being::DEMIGOD);
  addSyntax(1, "<race>");
  brief("Change the race of the Npc.");
  addOptions("race", ETRace::Instance().list());
  return;
}

bool NCmdRace::execute(Being* being, const std::vector<std::string>& args) {
  int race = 0;
  if ((race = ETRace::Instance().get(args[0])) == 0) {
    being->send("Invalid race.");
    return false;
  }
  avatar()->nedit()->race().set(race);
  avatar()->send("This Npc is now %s.\n", avatar()->nedit()->race().string().c_str());
  return true;
}

NCmdShortname::NCmdShortname(void) {
  name("shortname");
  level(Being::DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Npcs short name.");
  return;
}

bool NCmdShortname::execute(Being* being, const std::vector<std::string>& args) {
  avatar()->nedit()->identifiers().shortname(args[0]);
  avatar()->send("You've set the npc shortname to \"%s\".", avatar()->nedit()->identifiers().shortname().c_str());
  return true;
}
