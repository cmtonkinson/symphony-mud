/*
 * $Id: flagTable.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2009 Chris Tonkinson <cmtonkinson@gmail.com>
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

#include "flagTable.h"
#include "regex.h"

FlagTable::FlagTable(void) {
  return;
}

FlagTable::~FlagTable(void) {
  nameToValue().clear();
  valueToName().clear();
  return;
}

void FlagTable::add(const std::string& name, const FlagInt& value, std::string decorative) {
  nameToValue()[name] = value;
  valueToName()[value] = name;
  decorativeName()[value] = decorative;
  return;
}

FlagInt FlagTable::find(const std::string& name) {
  NameToVal::const_iterator it = nameToValue().find(name);
  if (it == nameToValue().end()) {
    return 0;
  } else {
    return it->second;
  }
}

FlagInt FlagTable::search(const std::string& name) {
  for (NameToVal::const_iterator it = nameToValue().begin(); it != nameToValue().end(); ++it) {
    if (Regex::strPrefix(name, it->first)) {
      return it->second;
    }
  }
  return 0;
}

const char* FlagTable::find(const FlagInt& value) {
  ValToName::const_iterator it = valueToName().find(value);
  if (it == valueToName().end()) {
    return "undefined";
  } else {
    return it->second.c_str();
  }
}

std::string FlagTable::dump(void) const {
  std::string buf;
  for (NameToVal::const_iterator it = nameToValue().begin(); it != nameToValue().end(); ++it) {
    buf.append(it->first).append(1, ' ');
  }
  if (!buf.empty()) {
    buf.resize(buf.size()-1);
  }
  return buf;
}

//////////////////////////////////////////////// Avatar
FTAvatarAdmin::FTAvatarAdmin(void) {
  add("headbuilder",   ADMIN_HEADBUILDER);
  add("incognito",     ADMIN_INCOGNITO);
  add("bigbrother",    ADMIN_BIGBROTHER);
  add("deaths",        ADMIN_BIGBRO_DEATHS);
  add("errors",        ADMIN_BIGBRO_ERRORS);
  add("logins",        ADMIN_BIGBRO_LOGINS);
  add("modes",         ADMIN_BIGBRO_MODES);
  add("resets",        ADMIN_BIGBRO_RESETS);
  add("system",        ADMIN_BIGBRO_SYSTEM);
  add("events",        ADMIN_BIGBRO_EVENTS);
  add("changes",       ADMIN_BIGBRO_CHANGES);
  return;
}

FTAvatarChannel::FTAvatarChannel(void) {
  add("newbie",  CHANNEL_NEWBIE);
  add("ooc",     CHANNEL_OOC);
  add("tell",    CHANNEL_TELL);
  return;
}

FTAvatarWho::FTAvatarWho(void) {
  add("afk",   WHO_AFK,   "{YAFK{x");
  add("busy",  WHO_BUSY,  "{RBUSY{x");
  add("bored", WHO_BORED, "{BB{CO{BR{CE{BD{x");
  add("dunce", WHO_DUNCE, "{RD{YU{GN{CC{ME{x");
  return;
}

//////////////////////////////////////////////// Command
FTCommand::FTCommand(void) {
  add("enabled",     COM_ENABLED);
  add("playeronly",  COM_PLAYERONLY);
  add("special",     COM_SPECIAL);
  add("social",      COM_SOCIAL);
  add("audible",     COM_AUDIBLE);
  add("tactile",     COM_TACTILE);
  return;
}

//////////////////////////////////////////////// Object
FTObject::FTObject(void) {
  add("noremove",  OBJECT_NOREMOVE);
  add("nodrop",    OBJECT_NODROP);
  add("noget",     OBJECT_NOGET);
  add("meltdrop",  OBJECT_MELTDROP);
  add("glowing",   OBJECT_GLOWING, "{x({Yglowing{x)");
  add("humming",   OBJECT_HUMMING, "{x({yhumming{x)");
  add("burnproof", OBJECT_BURNPROOF);
  return;
}

FTWeapon::FTWeapon(void) {
  add("burning",   WEAPON_BURNING);
  add("freezing",  WEAPON_FREEZING);
  add("shocking",  WEAPON_SHOCKING);
  add("sharp",     WEAPON_SHARP);
  return;
}

//////////////////////////////////////////////// Exit
FTExit::FTExit(void) {
  add("closeable", EXIT_CLOSEABLE);
  add("closed",    EXIT_CLOSED);
  add("lockable",  EXIT_LOCKABLE);
  add("locked",    EXIT_LOCKED);
  add("hidden",    EXIT_HIDDEN);
  return;
}

//////////////////////////////////////////////// Compound
FTCompound::FTCompound(void) {
  add("metal", COMPOUND_METAL);
  add("stone", COMPOUND_STONE);
  add("gem",   COMPOUND_GEM);
  add("wood",  COMPOUND_WOOD);
  add("skin",  COMPOUND_SKIN);
  add("cloth", COMPOUND_CLOTH);
  return;
}

//////////////////////////////////////////////// Room
FTRoom::FTRoom(void) {
  add("nomap",   ROOM_NOMAP);
  add("nogate",  ROOM_NOGATE);
  return;
}
