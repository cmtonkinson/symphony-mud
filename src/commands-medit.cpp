
#include "commands-medit.h"
#include "creature.h"
#include "display.h"
#include "io-handler.h"
#include "mob.h"
#include "room.h"

/*
MCmd::MCmd(void) {
  name("");
  level(DEMIGOD);
  addSyntax(0, "");
  brief("");
  return;
}

bool MCmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

MCmdClass::MCmdClass(void) {
  name("class");
  level(DEMIGOD);
  addSyntax(1, "<class>");
  brief("Change the class type of the Mob.");
  addOptions("class", ETPClass::Instance().list());
  return;
}

bool MCmdClass::execute(Creature* creature, const std::vector<std::string>& args) {
  int pclass = 0;
  if ((pclass = ETPClass::Instance().get(args[0])) == 0) {
    creature->send("Invalid class.");
    return false;
  }
  avatar()->medit()->pClass().set(pclass);
  avatar()->send("This Mob is now a %s.\n", avatar()->medit()->pClass().string().c_str());
  return true;
}

MCmdDescription::MCmdDescription(void) {
  name("description");
  level(DEMIGOD);
  addSyntax(0, "");
  brief("Invokes the Text Editor for the Mobs' description.");
  return;
}

bool MCmdDescription::execute(Creature* creature, const std::vector<std::string>& args) {
  IOHandler* h = new TeditIOHandler(creature);
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->medit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Mob Description"));
  h->getState()["pointer"] = (void*)avatar()->medit()->identifiers().descriptionp();
  avatar()->pushIOHandler(h);
  return true;
}

MCmdGender::MCmdGender(void) {
  name("gender");
  level(DEMIGOD);
  addSyntax(1, "<gender>");
  brief("Change the gender of the Mob.");
  addOptions("gender", ETGender::Instance().list());
  return;
}

bool MCmdGender::execute(Creature* creature, const std::vector<std::string>& args) {
  int gender = 0;
  if ((gender = ETGender::Instance().get(args[0])) == 0) {
    creature->send("Invalid gender.");
    return false;
  }
  avatar()->medit()->gender().set(gender);
  avatar()->send("This Mob is now %s.\n", avatar()->medit()->gender().string().c_str());
  return true;
}

MCmdInformation::MCmdInformation(void) {
  name("information");
  level(DEMIGOD);
  addSyntax(0, "");
  brief("Displays information about the Mob.");
  return;
}

bool MCmdInformation::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->send(Mob::getInformation(avatar()->medit()));
  return true;
}

MCmdKeywords::MCmdKeywords(void) {
  name("keywords");
  level(DEMIGOD);
  addSyntax(-1, "<key1 key2 key3 ...>");
  brief("Updtes the Mobs keywords.");
  return;
}

bool MCmdKeywords::execute(Creature* creature, const std::vector<std::string>& args) {
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
  level(DEMIGOD);
  addSyntax(1, "<number>");
  brief("Updates the Mobs level.");
  addOptions("level", "must be between 1 and 100, inclusive");
  return;
}

bool MCmdLevel::execute(Creature* creature, const std::vector<std::string>& args) {
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
  level(DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Mobs long name.");
  return;
}

bool MCmdLongname::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->medit()->identifiers().longname(args[0]);
  avatar()->send("You've set the mob longname to \"%s\".", avatar()->medit()->identifiers().longname().c_str());
  return true;
}

MCmdRace::MCmdRace(void) {
  name("race");
  level(DEMIGOD);
  addSyntax(1, "<race>");
  brief("Change the race of the Mob.");
  addOptions("race", ETRace::Instance().list());
  return;
}

bool MCmdRace::execute(Creature* creature, const std::vector<std::string>& args) {
  int race = 0;
  if ((race = ETRace::Instance().get(args[0])) == 0) {
    creature->send("Invalid race.");
    return false;
  }
  avatar()->medit()->race().set(race);
  avatar()->send("This Mob is now %s.\n", avatar()->medit()->race().string().c_str());
  return true;
}

MCmdShortname::MCmdShortname(void) {
  name("shortname");
  level(DEMIGOD);
  addSyntax(-1, "<string>");
  brief("Updates the Mobs short name.");
  return;
}

bool MCmdShortname::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->medit()->identifiers().shortname(args[0]);
  avatar()->send("You've set the mob shortname to \"%s\".", avatar()->medit()->identifiers().shortname().c_str());
  return true;
}
