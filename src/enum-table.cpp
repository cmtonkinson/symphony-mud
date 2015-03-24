
#include "enum-table.hpp"
#include "regex.hpp"

EnumTable::EnumTable(void) {
  return;
}

EnumTable::~EnumTable(void) {
  return;
}

void EnumTable::add(const EnumInt& n, const std::string& s) {
  _int2string[n] = s;
  _string2int[s] = n;
  return;
}

EnumInt EnumTable::get(const std::string& value) const {
  std::map<std::string,EnumInt>::const_iterator end = _string2int.end();
  for (std::map<std::string,EnumInt>::const_iterator it = _string2int.begin(); it != end; ++it) {
    if (Regex::strPrefix(value, it->first)) {
      return it->second;
    }
  }
  return 0;
}

std::string EnumTable::get(const EnumInt& value) const {
  std::map<EnumInt,std::string>::const_iterator it = _int2string.find(value);
  if (it != _int2string.end()) {
    return it->second;
  } else {
    return "error";
  }
}

std::string EnumTable::list(void) const {
  std::string output;
  std::map<std::string,EnumInt>::const_iterator end = _string2int.end();
  for (std::map<std::string,EnumInt>::const_iterator it = _string2int.begin(); it != end; ++it) {
    output.append(it->first).append(1, ' ');
  }
  if (!output.empty()) {
    output.resize(output.size()-1);
  }
  return output;
}

ETMode::ETMode(void) {
  add(MODE_NONE,   "none");
  add(MODE_ZEDIT,  "zedit");
  add(MODE_NEDIT,  "nedit");
  add(MODE_IEDIT,  "iedit");
  add(MODE_PEDIT,  "pedit");
  add(MODE_REDIT,  "redit");
  add(MODE_SEDIT,  "sedit");
  return;
}

ETStatus::ETStatus(void) {
  add(CONNECTING,  "connecting");
  add(CREATING,    "creating");
  add(DELETING,    "deleting");
  add(CONNECTED,   "connected");
  return;
}

ETComposing::ETComposing(void) {
  add(COMP_NOTE,   "note");
  return;
}

ETDirection::ETDirection(void) {
  add(NORTH, "north");
  add(EAST,  "east");
  add(SOUTH, "south");
  add(WEST,  "west");
  add(UP,    "up");
  add(DOWN,  "down");
  return;
}

ETFurniturePosition::ETFurniturePosition(void) {
  add(FURN_LAY_ON,   "lay_on");
  add(FURN_SIT_AT,   "sit_at");
  add(FURN_SIT_ON,   "sit_on");
  add(FURN_STAND_ON, "stand_on");
  return;
}

ETWeaponType::ETWeaponType(void) {
  add(WEAP_AXE,        "axe");
  add(WEAP_CLUB,       "club");
  add(WEAP_DAGGER,     "dagger");
  add(WEAP_FLAIL,      "flail");
  add(WEAP_GAUNTLET,   "gauntlet");
  add(WEAP_LONGSWORD,  "longsword");
  add(WEAP_MACE,       "mace");
  add(WEAP_POLEARM,    "polearm");
  add(WEAP_SHORTSWORD, "shortsword");
  add(WEAP_STAFF,      "staff");
  add(WEAP_WHIP,       "whip");
  return;
}

ETDamageVerb::ETDamageVerb(void) {
  // bash
  add(DAM_BASH,      "bash");
  add(DAM_BEATING,   "beating");
  add(DAM_CRUSH,     "crush");
  add(DAM_POUND,     "pound");
  add(DAM_PUNCH,     "punch");
  add(DAM_SLAM,      "slam");
  add(DAM_SMASH,     "smash");
  add(DAM_STRIKE,    "strike");
  add(DAM_THWACK,    "thwack");
  // slash
  add(DAM_CHOP,      "chop");
  add(DAM_CLEAVE,    "cleave");
  add(DAM_CUT,       "cut");
  add(DAM_HACK,      "hack");
  add(DAM_SLASH,     "slash");
  add(DAM_SLICE,     "slice");
  // pierce
  add(DAM_BITE,      "bite");
  add(DAM_PIERCE,    "pierce");
  add(DAM_PRICK,     "prick");
  add(DAM_PUNCTURE,  "puncture");
  add(DAM_STAB,      "stab");
  add(DAM_STING,     "sting");
  add(DAM_THRUST,    "thrust");
  // exotic
  add(DAM_WRATH,     "wrath");
  return;
}

ETPosition::ETPosition(void) {
  add(LAYING,    "laying");
  add(SITTING,   "sitting");
  add(STANDING,  "standing");
  add(MOUNTED,   "mounted");
  return;
}

ETAction::ETAction(void) {
  add(DEAD,      "dead");
  add(SLEEPING,  "sleeping");
  add(FIGHTING,  "fighting");
  return;
}

ETGender::ETGender(void) {
  add(NEUTRAL, "neutral");
  add(MALE,    "male");
  add(FEMALE,  "female");
  return;
}

ETRace::ETRace(void) {
  add(DWARF,   "dwarf");
  add(ELF,     "elf");
  add(HUMAN,   "human");
  return;
}

ETPClass::ETPClass(void) {
  add(CLERIC,  "cleric");
  add(MAGE,    "mage");
  add(ROGUE,   "rogue");
  add(WARRIOR, "warrior");
  return;
}
