
#include <cstring>
#include "avatar.h"
#include "board.h"
#include "command.h"
#include "enumTable.h"
#include "exit.h"
#include "item-types.h"
#include "item.h"
#include "note.h"
#include "npc.h"
#include "placement.h"
#include "placementItem.h"
#include "placementNpc.h"
#include "room.h"
#include "storage.h"
#include "zone.h"

const unsigned Storage::LOAD_DONE;
const unsigned Storage::LOAD_NULL;
const unsigned Storage::LOAD_NEW;

/***************************************************************************************************
 * ZONE
 **************************************************************************************************/
void Storage::write(FILE* fp, Zone* zone) {
  BEGIN("ZONE")
  out(fp, "low",          zone->low());
  out(fp, "high",         zone->high());
  out(fp, "name",         zone->name());
  out(fp, "terrain",      zone->terrain()->name());
  out(fp, "builders",     zone->serializeBuilders());
  for (auto iter : zone->rooms())   write(fp, iter.second);
  for (auto iter : zone->items()) write(fp, iter.second);
  for (auto iter : zone->npcs())    write(fp, iter.second);
  END("ZONE")
  return;
}

bool Storage::read(FILE* fp, Zone* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "ZONE", [&fp, &loading, &input]() {
    STORE_CASE("low",       &Zone::low)
    STORE_CASE("high",      &Zone::high)
    STORE_CASE("name",      &Zone::name)
    STORE_CASE("terrain",   &Zone::setTerrain)
    STORE_CASE_STRING("builders",   loading->unserializeBuilders(str);)
    STORE_DESCEND_NEW("ROOM",   Room,   loading->insert(instance);)
    STORE_DESCEND_NEW("ITEM", Item, loading->insert(instance);)
    STORE_DESCEND_NEW("NPC",    Npc,    loading->insert(instance);)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * ROOM
 **************************************************************************************************/
void Storage::write(FILE* fp, Room* room) {
  BEGIN("ROOM")
  out(fp, "vnum",         room->vnum());
  out(fp, "name",         room->name());
  out(fp, "description",  room->description());
  out(fp, "smell",        room->smell());
  out(fp, "sound",        room->sound());
  out(fp, "terrain",      room->terrain()->name());
  for (auto iter : room->placements()) write(fp, iter);
  for (unsigned u = 0; u < 6; ++u) {
    if (room->exit(u)) write(fp, room->exit(u));
  }
  END("ROOM\n")
  return;
}

bool Storage::read(FILE* fp, Room* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "ROOM", [&fp, &loading, &input]() {
    STORE_CASE("vnum",        &Room::vnum)
    STORE_CASE("name",        &Room::name)
    STORE_CASE("description", &Room::description)
    STORE_CASE("smell",       &Room::smell)
    STORE_CASE("sound",       &Room::sound)
    STORE_CASE("terrain",     &Room::setTerrain)
    STORE_DESCEND_NEW("EXIT", Exit,
      loading->exit(instance->direction().number(), instance);
    )
    STORE_DESCEND_NEW("PLACEMENT_ITEM", PlacementItem,
      loading->placements().push_back(instance);
      instance->room(loading);
    )
    STORE_DESCEND_NEW("PLACEMENT_NPC", PlacementNpc,
      loading->placements().push_back(instance);
      instance->room(loading);
    )
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * EXIT
 **************************************************************************************************/
void Storage::write(FILE* fp, Exit* exit) {
  BEGIN("EXIT")
  out(fp, "targetVnum", exit->targetVnum());
  out(fp, "direction",  exit->direction());
  out(fp, "key",        exit->key());
  out(fp, "flags",      exit->flags().value());
  END("EXIT")
  return;
}

bool Storage::read(FILE* fp, Exit* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "EXIT", [&fp, &loading, &input]() {
    STORE_CASE("targetVnum",  &Exit::targetVnum)
    STORE_CASE("direction",   &Exit::direction)
    STORE_CASE("key",         &Exit::key)
    STORE_CASE_WITH_CODE("flags",     unsigned long,  "%lu",  loading->flags().value(val);)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * PLACEMENT
 **************************************************************************************************/
void Storage::write(FILE* fp, Placement* rule) {
  char buffer[32];
  sprintf(buffer, "PLACEMENT_%s", rule->strType());
  BEGIN(buffer)
  out(fp, "target",       rule->target());
  out(fp, "number",       rule->number());
  out(fp, "max",          rule->max());
  out(fp, "probability",  rule->probability());
  switch (rule->type()) {
    case Placement::ITEM:
      out(fp, "preposition",          ((PlacementItem*)rule)->preposition());
      out(fp, "indirectItem",       ((PlacementItem*)rule)->indirectItem());
      out(fp, "indirectItemIndex",  ((PlacementItem*)rule)->indirectItemIndex());
      break;
  }
  END(buffer)
  return;
}

bool Storage::read(FILE* fp, Placement* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "PLACEMENT_", [&fp, &loading, &input]() {
    STORE_CASE("target",              &Placement::target)
    STORE_CASE("number",              &PlacementItem::number)
    STORE_CASE("max",                 &PlacementItem::max)
    STORE_CASE("probability",         &PlacementItem::probability)
    switch (loading->type()) {
      case Placement::ITEM:
        STORE_CASE_WITH_TYPE("preposition",         &PlacementItem::preposition,         PlacementItem)
        STORE_CASE_WITH_TYPE("indirectItem",      &PlacementItem::indirectItem,      PlacementItem)
        STORE_CASE_WITH_TYPE("indirectItemIndex", &PlacementItem::indirectItemIndex, PlacementItem)
        break;
    }
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * ITEM
 **************************************************************************************************/
void Storage::write(FILE* fp, Item* item, const char* suffix) {
  char token[32];
  if (suffix != nullptr) {
    sprintf(token, "ITEM_%s", suffix);
  } else {
    sprintf(token, "ITEM");
  }
  BEGIN(token)
  out(fp, "vnum",         item->vnum());
  out(fp, "level",        item->level());
  out(fp, "value",        item->value());
  out(fp, "type",         item->type());
  out(fp, "wearable",     item->wearable());
  out(fp, "flags",        item->flags().value());
  out(fp, "shortname",    item->identifiers().shortname());
  out(fp, "longname",     item->identifiers().longname());
  out(fp, "description",  item->identifiers().description());
  out(fp, "keywords",     item->identifiers().serializeKeywords());
  out(fp, "modifiers",    item->serializeModifiers());
  out(fp, "composition",  item->serializeComposition());
  switch (item->type()) {
    case Item::Type_Furniture:
      out(fp, "furniture_capacity", item->furniture()->capacity());
      out(fp, "furniture_layOn",    item->furniture()->layOn());
      out(fp, "furniture_sitAt",    item->furniture()->sitAt());
      out(fp, "furniture_sitOn",    item->furniture()->sitOn());
      out(fp, "furniture_standOn",  item->furniture()->standOn());
      break;
    case Item::Type_Weapon:
      out(fp, "weapon_type",     item->weapon()->type().string());
      out(fp, "weapon_verb",     item->weapon()->verb().string());
      out(fp, "weapon_damage",   item->weapon()->damage().serialize());
      break;
    case Item::Type_Container:
      for (auto iter : item->container()->inventory().itemList()) write(fp, iter, "BAG");
      break;
    default:
      break;
  }
  END(token)
  return;
}

bool Storage::read(FILE* fp, Item* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "ITEM", [&fp, &loading, &input]() {
    STORE_CASE("vnum",                &Item::vnum)
    STORE_CASE("level",               &Item::level)
    STORE_CASE("value",               &Item::value)
    STORE_CASE_WITH_CODE("type",      unsigned,       "%u",   loading->type(static_cast<Item::Type>(val));)
    STORE_CASE_WITH_CODE("wearable",  unsigned,       "%u",   loading->wearable(static_cast<Item::Wearable>(val));)
    STORE_CASE_WITH_CODE("flags",     unsigned long,  "%lu",  loading->flags().value(val);)
    STORE_CASE_STRING("shortname",    loading->identifiers().shortname(str);)
    STORE_CASE_STRING("longname",     loading->identifiers().longname(str);)
    STORE_CASE_STRING("description",  loading->identifiers().description(str);)
    STORE_CASE_STRING("keywords",     loading->identifiers().unserializeKeywords(str);)
    STORE_CASE_STRING("modifiers",    loading->unserializeModifiers(str);)
    STORE_CASE_STRING("composition",  loading->unserializeComposition(str);)
    switch (loading->type()) {
      case Item::Type_Furniture:
        STORE_CASE_WITH_CODE("furniture_capacity",  unsigned, "%u", loading->furniture()->capacity(val);)
        STORE_CASE_WITH_CODE("furniture_layOn",     unsigned, "%u", loading->furniture()->layOn(val);)
        STORE_CASE_WITH_CODE("furniture_sitAt",     unsigned, "%u", loading->furniture()->sitAt(val);)
        STORE_CASE_WITH_CODE("furniture_sitOn",     unsigned, "%u", loading->furniture()->sitOn(val);)
        STORE_CASE_WITH_CODE("furniture_standOn",   unsigned, "%u", loading->furniture()->standOn(val);)
        break;
      case Item::Type_Weapon:
        STORE_CASE_STRING("weapon_type",    loading->weapon()->type().set(ETWeaponType::Instance().get(str));)
        STORE_CASE_STRING("weapon_verb",    loading->weapon()->verb().set(ETDamageVerb::Instance().get(str));)
        STORE_CASE_STRING("weapon_damage",  loading->weapon()->damage().unserialize(str);)
        break;
      case Item::Type_Container:
        STORE_DESCEND_NEW("ITEM_BAG", Item, loading->container()->inventory().add(instance);)
        break;
      default:
        break;
    }
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * NPC
 **************************************************************************************************/
void Storage::write(FILE* fp, Npc* npc) {
  BEGIN("NPC")
  out(fp, "vnum",           npc->vnum());
  out(fp, "mobility",       npc->mobility());
  out(fp, "aggressiveness", npc->aggressiveness());
  write_base(fp, npc);
  END("NPC")
  return;
}

bool Storage::read(FILE* fp, Npc* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "NPC", [&fp, &loading, &input]() {
    STORE_CASE("vnum",           &Npc::vnum)
    STORE_CASE("mobility",       &Npc::mobility)
    STORE_CASE("aggressiveness", &Npc::aggressiveness)
    STORE_DESCEND("BEING")
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * AVATAR
 **************************************************************************************************/
void Storage::write(FILE* fp, Avatar* avatar) {
  BEGIN("AVATAR")
  out(fp, "password",         avatar->password());
  out(fp, "gechoColor",       avatar->gechoColor());
  out(fp, "title",            avatar->title());
  out(fp, "poofin",           avatar->poofin());
  out(fp, "poofout",          avatar->poofout());
  out(fp, "age",              avatar->age());
  out(fp, "bankGold",         avatar->bankGold());
  out(fp, "bankSilver",       avatar->bankSilver());
  out(fp, "bigBrother",       avatar->bigBrother());
  out(fp, "adminFlags",       avatar->adminFlags().value());
  out(fp, "channelFlags",     avatar->channelFlags().value());
  out(fp, "whoFlags",         avatar->whoFlags().value());
  out(fp, "roomNumber",       avatar->roomNumber());
  out(fp, "deletionStatus",   avatar->deletionStatus());
  write_base(fp, avatar);
  END("AVATAR")
  return;
}

bool Storage::read(FILE* fp, Avatar* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "AVATAR", [&fp, &loading, &input]() {
    STORE_CASE_STRING("password", loading->password(str);)
    STORE_CASE("gechoColor",      &Avatar::gechoColor)
    STORE_CASE("title",           &Avatar::title)
    STORE_CASE("poofin",          &Avatar::poofin)
    STORE_CASE("poofout",         &Avatar::poofout)
    STORE_CASE("age",             &Avatar::age)
    STORE_CASE("bankGold",        &Avatar::bankGold)
    STORE_CASE("bankSilver",      &Avatar::bankSilver)
    STORE_CASE("bigBrother",      &Avatar::bigBrother)
    STORE_CASE("roomNumber",      &Avatar::roomNumber)
    STORE_CASE("deletionStatus",  &Avatar::deletionStatus)
    STORE_CASE_WITH_CODE("adminFlags",    unsigned, "%u", loading->adminFlags().value(val);)
    STORE_CASE_WITH_CODE("channelFlags",  unsigned, "%u", loading->channelFlags().value(val);)
    STORE_CASE_WITH_CODE("whoFlags",      unsigned, "%u", loading->whoFlags().value(val);)
    STORE_DESCEND("BEING")
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * BEING
 **************************************************************************************************/
void Storage::write_base(FILE* fp, Being* being) {
  BEGIN("BEING")
  out(fp, "shortname",    being->identifiers().shortname());
  out(fp, "longname",     being->identifiers().longname());
  out(fp, "description",  being->identifiers().description());
  out(fp, "keywords",     being->identifiers().serializeKeywords());
  fprintf(fp, "identity %s %s %s\n",
    being->gender().string().c_str(),
    being->race().string().c_str(),
    being->pClass().string().c_str()
  );
  fprintf(fp, "level %u %u %u\n",
    being->level(),
    being->exp(),
    being->tnl()
  );
  fprintf(fp, "health %d/%d %d/%d %d\n",
    being->health(),
    being->maxHealth(),
    being->mana(),
    being->maxMana(),
    being->stamina()
  );
  fprintf(fp, "stats %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu\n",
    being->strength(),
    being->maxStrength(),
    being->dexterity(),
    being->maxDexterity(),
    being->constitution(),
    being->maxConstitution(),
    being->intelligence(),
    being->maxIntelligence(),
    being->focus(),
    being->maxFocus(),
    being->creativity(),
    being->maxCreativity(),
    being->charisma(),
    being->maxCharisma(),
    being->luck(),
    being->maxLuck()
  );
  fprintf(fp, "armor %d %d %d %d %d\n",
    being->armor(),
    being->bash(),
    being->slash(),
    being->pierce(),
    being->exotic()
  );
  out(fp, "trains",     being->trains());
  out(fp, "gold",       being->gold());
  out(fp, "silver",     being->silver());
  out(fp, "abilities",  being->serializeAbilities());
  for (auto iter : being->inventory().itemList()) write(fp, iter, "INV");
  for (auto iter : being->equipment().itemMap()) write(fp, iter.second, "EQ");
  END("BEING")
  return;
}

bool Storage::read_base(FILE* fp, Being* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "BEING", [&fp, &loading, &input]() {
    STORE_CASE_STRING("shortname",    loading->identifiers().shortname(str);)
    STORE_CASE_STRING("longname",     loading->identifiers().longname(str);)
    STORE_CASE_STRING("description",  loading->identifiers().description(str);)
    STORE_CASE_STRING("keywords",     loading->identifiers().unserializeKeywords(str);)
    if (strcmp("identity", input) == 0) {
      char gender[32], race[32], klass[32];
      fscanf(fp, "%s %s %s", gender, race, klass);
      loading->gender().set(ETGender::Instance().get(gender));
      loading->race().set(ETRace::Instance().get(race));
      loading->pClass().set(ETPClass::Instance().get(klass));
      return;
    }
    if (strcmp("level", input) == 0) {
      unsigned level, exp, tnl;
      fscanf(fp, "%u %u %u", &level, &exp, &tnl);
      loading->level(level);
      loading->exp(exp);
      loading->tnl(tnl);
    }
    if (strcmp("health", input) == 0) {
      int health, maxHealth, mana, maxMana, stamina;
      fscanf(fp, "%d/%d %d/%d %d", &health, &maxHealth, &mana, &maxMana, &stamina);
      loading->maxHealth(maxHealth);
      loading->health(health);
      loading->maxMana(maxMana);
      loading->mana(mana);
      loading->stamina(stamina);
    }
    if (strcmp("stats", input) == 0) {
      int strength, maxStrength, dexterity, maxDexterity, constitution, maxConstitution, intelligence, maxIntelligence, focus, maxFocus, creativity, maxCreativity, charisma, maxCharisma, luck, maxLuck;
      fscanf(fp, " %d/%d %d/%d %d/%d %d/%d %d/%d %d/%d %d/%d %d/%d",
        &strength, &maxStrength, &dexterity, &maxDexterity, &constitution, &maxConstitution, &intelligence, &maxIntelligence, &focus, &maxFocus, &creativity, &maxCreativity, &charisma, &maxCharisma, &luck, &maxLuck
      );
      loading->maxStrength(maxStrength);
      loading->strength(strength);
      loading->maxDexterity(maxDexterity);
      loading->dexterity(dexterity);
      loading->maxConstitution(maxConstitution);
      loading->constitution(constitution);
      loading->maxIntelligence(maxIntelligence);
      loading->intelligence(intelligence);
      loading->maxFocus(maxFocus);
      loading->focus(focus);
      loading->maxCreativity(maxCreativity);
      loading->creativity(creativity);
      loading->maxCharisma(maxCharisma);
      loading->charisma(charisma);
      loading->maxLuck(maxLuck);
      loading->luck(luck);
    }
    if (strcmp("armor", input) == 0) {
      int armor, bash, slash, pierce, exotic;
      fscanf(fp, "%d %d %d %d %d", &armor, &bash, &slash, &pierce, &exotic);
      loading->armor(armor);
      loading->bash(bash);
      loading->slash(slash);
      loading->pierce(pierce);
      loading->exotic(exotic);
    }
    STORE_CASE("trains",  &Being::trains)
    STORE_CASE("gold",    &Being::gold)
    STORE_CASE("silver",  &Being::silver)
    STORE_CASE_STRING("abilities",    loading->unserializeAbilities(str);)
    STORE_DESCEND_NEW("ITEM_INV", Item, loading->inventory().add(instance);)
    STORE_DESCEND_NEW("ITEM_EQ", Item,
      Item* removed     = nullptr;
      std::string message = "";
      loading->wear(instance, message, removed);
    )
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * SOCIAL COMMAND
 **************************************************************************************************/
void Storage::write(FILE* fp, SocialCommand* social) {
  BEGIN("SOCIAL")
  out(fp, "name",         social->name());
  out(fp, "creator",      social->creator());
  out(fp, "targetNone",   social->targetNone());
  out(fp, "targetSelf",   social->targetSelf());
  out(fp, "targetVictim", social->targetVictim());
  out(fp, "noneActor",    social->noneActor());
  out(fp, "noneRoom",     social->noneRoom());
  out(fp, "selfActor",    social->selfActor());
  out(fp, "selfRoom",     social->selfRoom());
  out(fp, "victimActor",  social->victimActor());
  out(fp, "victimVictim", social->victimVictim());
  out(fp, "victimRoom",   social->victimRoom());
  out(fp, "flags",        social->flags().value());
  END("SOCIAL")
  return;
}

bool Storage::read(FILE* fp, SocialCommand* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "SOCIAL", [&fp, &loading, &input]() {
    STORE_CASE_STRING("name",     loading->name(str);)
    STORE_CASE_STRING("creator",  loading->creator(str);)
    STORE_CASE("targetNone",    &SocialCommand::targetNone)
    STORE_CASE("targetSelf",    &SocialCommand::targetSelf)
    STORE_CASE("targetVictim",  &SocialCommand::targetVictim)
    STORE_CASE_STRING("noneActor",    loading->noneActor(str);)
    STORE_CASE_STRING("noneRoom",     loading->noneRoom(str);)
    STORE_CASE_STRING("selfActor",    loading->selfActor(str);)
    STORE_CASE_STRING("selfRoom",     loading->selfRoom(str);)
    STORE_CASE_STRING("victimActor",  loading->victimActor(str);)
    STORE_CASE_STRING("victimVictim", loading->victimVictim(str);)
    STORE_CASE_STRING("victimRoom",   loading->victimRoom(str);)
    STORE_CASE_WITH_CODE("flags", unsigned long, "%lu", loading->flags().value(val);)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * BOARD
 **************************************************************************************************/
void Storage::write(FILE* fp, Board* board) {
  BEGIN("BOARD")
  out(fp, "number", board->number());
  for (auto iter : board->notes()) write(fp, iter.second);
  END("BOARD")
  return;
}

bool Storage::read(FILE* fp, Board* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "BOARD", [&fp, &loading, &input]() {
    STORE_CASE("number", &Board::number)
    STORE_DESCEND_NEW("NOTE", Note,
      instance->board(loading->number());
      loading->notes().insert(std::make_pair(instance->ID(), instance));
    )
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * NOTE
 **************************************************************************************************/
void Storage::write(FILE* fp, Note* note) {
  BEGIN("NOTE")
  out(fp, "ID",      note->ID());
  out(fp, "board",   note->board());
  out(fp, "author",  note->author());
  out(fp, "subject", note->subject());
  out(fp, "body",    note->body());
  END("NOTE")
  return;
}

bool Storage::read(FILE* fp, Note* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = read_inner(fp, loading, input, "NOTE", [&fp, &loading, &input]() {
    STORE_CASE("ID",    &Note::ID)
    STORE_CASE("board", &Note::board)
    STORE_CASE_STRING("author",   loading->author(str);)
    STORE_CASE_STRING("subject",  loading->subject(str);)
    STORE_CASE_STRING("body",     loading->body(str);)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * INTERNAL METHODS
 **************************************************************************************************/
unsigned Storage::read_inner(FILE* fp, void* loading, char* input, const char* boundary, voidFunc lambda) {
  int next = 0;
  char end_boundary[32];
  sprintf(end_boundary, "/%s", boundary);

  // The first input should be a prefix match on the boundary for the item.
  fscanf(fp, "%s", input);
  if (feof(fp) || strstr(input, boundary) == NULL) return LOAD_NULL;

  // Iteratively look for new keys.
  while (!feof(fp)) {
    // Get the input.
    fscanf(fp, "%s", input);
    // Stop if this is the end token.
    if (strstr(input, end_boundary) != NULL) break;
    // Look for valid keys.
    lambda();
    // Test the stream (and reset the pointer).
    next = peek(fp)[0];
    // If the next character is capitalized, switch modes.
    if (std::isupper(next)) return LOAD_NEW;
    // The next input looks like a key - keep iterating.
  }
  return LOAD_DONE;
}

std::string Storage::read_string(FILE* fp) {
  std::string output;
  size_t length = 0;
  char* buffer    = nullptr;
  // Find the length of the string and allocate a buffer.
  fscanf(fp, "%zu:", &length);
  if (length < 1) return ""; // Empty string
  buffer = new char[length + 1];
  // Read the string and terminate with null.
  fgets(buffer, length + 1, fp);
  buffer[length] = 0;
  // Convert the data to a string and clean up.
  output = buffer;
  delete buffer;
  return output;
}

std::string Storage::peek(FILE* fp) {
  char buf[32];
  fgets(buf, 32, fp);
  fseek(fp, -strlen(buf), SEEK_CUR);
  return buf;
}

void Storage::out(FILE* fp, const char* key, bool value) {
  fprintf(fp, "%s %c\n", key, (value ? 'T' : 'F'));
  return;
}

void Storage::out(FILE* fp, const char* key, const char* value) {
  fprintf(fp, "%s %zu:%s\n", key, strlen(value), value);
  return;
}

template <class ItemType>
void Storage::in(FILE* fp, ItemType* item, void (ItemType::*method)(bool)) {
  char x;
  fscanf(fp, " %c", &x);
  (*item.*method)(x == 'T');
  return;
}

template <class ItemType>
void Storage::in(FILE* fp, ItemType* item, void (ItemType::*method)(const char*)) {
  std::string buf = read_string(fp);
  (*item.*method)(buf.c_str());
  return;
}

template <class ItemType>
void Storage::in(FILE* fp, ItemType* item, void (ItemType::*method)(std::string)) {
  std::string buf = read_string(fp);
  (*item.*method)(buf);
  return;
}
