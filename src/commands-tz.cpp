
#include "area.h"
#include "avatar.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "container.h"
#include "display.h"
#include "group.h"
#include "io-handler.h"
#include "room.h"
#include "world.h"

CmdTell::CmdTell(void) {
  name("tell");
  playerOnly(true);
  addSyntax(0, "");
  addSyntax(-2, "<target> <message>");
  brief("Sends the message to the target on the Out-Of-Charater 'tell' channel.");
  return;
}

bool CmdTell::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* target = NULL;

  if (args[0].empty()) {
    avatar()->channelFlags().toggle(CHANNEL_TELL);
    if (avatar()->channelFlags().test(CHANNEL_TELL)) {
      avatar()->send("Tell channel [{GON{x]");
    } else {
      avatar()->send("Tell channel [{rOFF{x]");
    }
    return true;
  }

  if ((target = World::Instance().findAvatar(args[0])) == NULL) {
    avatar()->send("They're not around at the moment.");
    return false;
  } else {
    if (avatar() == target) {
      avatar()->send("That would be fairly pointless, no?");
      return false;
    }
    if (!(avatar()->canSee(target) == Creature::SEE_NAME)) {
      avatar()->send("They're not around at the moment.");
      return false;
    }
    if (!target->channelFlags().test(CHANNEL_TELL)) {
      avatar()->send("They're not receiving tells right now.");
      return false;
    }
    target->replyTo(avatar()->identifiers().shortname());
    target->send("%s tells you, \"{Y%s{x\"", target->seeName(avatar(), true).c_str(), args[1].c_str());
    avatar()->send("You tell %s, \"{Y%s{x\"", target->identifiers().shortname().c_str(), args[1].c_str());
  }

  return true;
}

CmdTime::CmdTime(void) {
  name("time");
  addSyntax(0, "");
  brief("Displays the current game time.");
  return;
}

bool CmdTime::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string output;
  output.append("Blah blah blah - haven't figured this out yet (but coming soon!)\n\n");
  creature->send(output);
  return true;
}

CmdTimers::CmdTimers(void) {
  name("timers");
  level(Creature::DEMIGOD);
  addSyntax(0, "");
  brief("Displays the status on timers set to fire in the near future.");
  return;
}

bool CmdTimers::execute(Creature* creature, const std::vector<std::string>& args) {
  creature->send("This command has been temporarily broken.");
  return true;
}

CmdTitle::CmdTitle(void) {
  name("title");
  addSyntax(0, "");
  addSyntax(1, "clear");
  addSyntax(-1, "<string>");
  brief("Displays or updates your player 'Who list' title.");
  return;
}

bool CmdTitle::execute(Creature* creature, const std::vector<std::string>& args) {
  Avatar* avatar = (Avatar*)creature;
  std::string title = args[0];

  if (title.empty()) {
    avatar->send("Current title: %s", avatar->title());
    return true;
  }

  if (title.compare("clear") == 0) {
    avatar->title("");
    avatar->send("Your title has been removed.");
    return true;
  }

  avatar->title(title);
  avatar->send("Your new title is: %s{x", avatar->title());
  return true;
}

CmdUngroup::CmdUngroup(void) {
  name("ungroup");
  playerOnly(true);
  addSyntax(0, "");
  brief("Remove yourself from your current group.");
  return;
}

bool CmdUngroup::execute(Creature* creature, const std::vector<std::string>& args) {
  if (creature->group()->members().size() == 1) {
    creature->send("You're already solo.");
  } else {
    creature->ungroup();
  }
  return true;
}

CmdUnlock::CmdUnlock(void) {
  name("unlock");
  addSyntax(1, "<direction>");
  brief("Unlocks a door.");
  return;
}

bool CmdUnlock::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> foo;
  Exit* exit = NULL;
  Object* key = NULL;

  if ((exit = creature->room()->exit(args[0])) == NULL) {
    creature->send("There is no door in that direction.");
    return false;
  }
  if (!exit->flag(EXIT_CLOSED)) {
    creature->send("It's open.");
    return false;
  }
  if (!exit->flag(EXIT_LOCKED)) {
    creature->send("It's already unlocked.");
    return false;
  }
  foo = creature->inventory().searchObjects(exit->key());
  if (!foo.empty()) {
    key = foo.front();
  }
  if (key == NULL || !key->isKey()) {
    creature->send("You don't have the key.");
    return false;
  }

  exit->flag(EXIT_LOCKED, false);
  creature->send("You unlock the door.");
  creature->room()->send_cond("$p unlocked the way $e.", creature, exit);
  return true;
}

CmdWear::CmdWear(void) {
  name("wear");
  addSyntax(1, "<object>");
  brief("Equips an object.");
  return;
}

bool CmdWear::execute(Creature* creature, const std::vector<std::string>& args) {
  std::list<Object*> objects;
  Object* removed = NULL;
  std::string error;

  objects = creature->inventory().searchObjects(args[0]);
  if (!objects.empty()) {
    for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
      if (creature->wear(*it, error, removed)) {
        creature->inventory().remove(*it);
        if (removed) {
          creature->send("You remove %s{x.\n", removed->identifiers().shortname().c_str());
          creature->room()->send_cond("$p removes $o.\n", creature, removed);
        }
        creature->send("You wear %s{x.\n", (*it)->identifiers().shortname().c_str());
        creature->room()->send_cond("$p wears $o.\n", creature, *it);
      } else {
        creature->send(error);
      }
    }
  } else {
    creature->send("You don't have that.");
    return false;
  }

  return true;
}

CmdWho::CmdWho(void) {
  name("who");
  addSyntax(0, "");
  brief("Display names of players who are currently connected.");
  return;
}

bool CmdWho::execute(Creature* creature, const std::vector<std::string>& args) {
  std::map<std::string,Avatar*>::const_iterator it;
  std::string output;
  std::string who;

  output.append("     Players Online\n");
  for (it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it) {
    if (it->second->isConnected() && creature->canSee(it->second) == Creature::SEE_NAME) {
      output.append("  ");
      if (it->second->adminFlags().test(ADMIN_INCOGNITO)) {
        output.append("{WINVIS{x ");
      }
      who = it->second->listWhoFlags();
      if (!who.empty()) {
        output.append(who).append(1, ' ');
      }
      output.append(it->second->identifiers().shortname());
      output.append(it->second->title());
      output.append("{x\n");
    }
  }
  creature->send(output);
  return true;
}

CmdWhoAmI::CmdWhoAmI(void) {
  name("whoami");
  addSyntax(0, "");
  brief("Displays your account name.");
  return;
}

bool CmdWhoAmI::execute(Creature* creature, const std::vector<std::string>& args) {
  creature->send("You are logged in as %s.", creature->identifiers().shortname().c_str());
  return true;
}

CmdWorldSave::CmdWorldSave(void) {
  name("worldsave");
  playerOnly(true);
  level(Creature::DEMIGOD);
  addSyntax(0, "");
  brief("Saves the whole world! (but don't get cocky, it's not really that heroic)");
  return;
}

bool CmdWorldSave::execute(Creature* creature, const std::vector<std::string>& args) {
  World::Instance().save();
  avatar()->send("World areas and player profiles saved successfully.");
  return true;
}

CmdTest::CmdTest(void) {
  name("test");
  level(Creature::CREATOR);
  addSyntax(0, "");
  brief("A command so that an admin can live-test arbitrary code fragments.");
  return;
}

/*
 * No, really.  This command exists soley so that a developer can live-test
 * arbitrary code fragments without exposing any half-baked Command logic
 * to the populace.
 */
#include <fstream>
#include "loadRule.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "object-furniture.h"
#include "object-weapon.h"
#include "object.h"
#include "storage.h"

bool CmdTest::execute(Creature* creature, const std::vector<std::string>& args) {
  unsigned status = 0;
  FILE* fp = 0;

  Room* r = nullptr;
  fp = fopen("room.txt", "w");
  Storage::dump(fp, creature->room());
  fclose(fp);
  std::ifstream room_ifs("room.txt");
  std::string room_content( (std::istreambuf_iterator<char>(room_ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped Room...\n");
  creature->send(room_content);
  creature->send("\nLoaded Room...\n");
  fp = fopen("room.txt", "r");
  while (!feof(fp)) {
    r = new Room();
    status = Storage::load(fp, r);
    if (status) {
      creature->send("Room(%x)::vnum        = %lu\n", r, r->vnum());
      creature->send("Room(%x)::name        = %s\n", r, r->name().c_str());
      creature->send("Room(%x)::description = %s\n", r, r->description().c_str());
      creature->send("Room(%x)::smell       = %s\n", r, r->smell().c_str());
      creature->send("Room(%x)::sound       = %s\n", r, r->sound().c_str());
      creature->send("Room(%x)::terrain     = %s\n", r, r->terrain()->name().c_str());
      for (unsigned u = 0; u < 6; ++u) {
        if (r->exit(u) != nullptr) {
          creature->send("Room(%x)::Exit(%x)::direction = %u\n", r, r->exit(u), r->exit(u)->direction().number());
          creature->send("Room(%x)::Exit(%x)::key       = %lu\n", r, r->exit(u), r->exit(u)->key());
        }
      }
      for (auto iter : r->loadRules()) {
        creature->send("Room(%x)::LoadRule(%x)::type        = %s\n",  r, iter, iter->strType());
        creature->send("Room(%x)::LoadRule(%x)::target      = %lu\n", r, iter, iter->target());
        creature->send("Room(%x)::LoadRule(%x)::number      = %u\n",  r, iter, iter->number());
        creature->send("Room(%x)::LoadRule(%x)::max         = %u\n",  r, iter, iter->max());
        creature->send("Room(%x)::LoadRule(%x)::probability = %u\n",  r, iter, iter->probability());
      }
    }
  }
  fclose(fp);

  Area* a = nullptr;
  fp = fopen("area.txt", "w");
  Storage::dump(fp, creature->room()->area());
  fclose(fp);
  std::ifstream area_ifs("area.txt");
  std::string area_content( (std::istreambuf_iterator<char>(area_ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped Area...\n");
  creature->send(area_content);
  creature->send("\nLoaded Area...\n");
  fp = fopen("area.txt", "r");
  while (!feof(fp)) {
    a = new Area();
    status = Storage::load(fp, a);
    if (status) {
      creature->send("Area(%x)::low     = %lu\n", a, a->low());
      creature->send("Area(%x)::high    = %lu\n", a, a->high());
      creature->send("Room(%x)::name    = %s\n",  a, a->name().c_str());
      creature->send("Room(%x)::terrain = %s\n",  a, a->terrain()->name().c_str());
    }
  }
  fclose(fp);

  fp = fopen("object.txt", "w");
  for (auto iter : creature->inventory().objectList()) Storage::dump(fp, iter);
  fclose(fp);
  std::ifstream object_ifs("object.txt");
  std::string object_content( (std::istreambuf_iterator<char>(object_ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped Objects...\n");
  creature->send(object_content);
  Object* o = nullptr;
  creature->send("\nLoaded Objects...\n");
  fp = fopen("object.txt", "r");
  while (!feof(fp)) {
    o = new Object();
    status = Storage::load(fp, o);
    if (status) {
      creature->send("Object(%x)::vnum        = %lu\n", o, o->vnum());
      creature->send("Object(%x)::level       = %u\n",  o, o->level());
      creature->send("Object(%x)::value       = %u\n",  o, o->value());
      creature->send("Object(%x)::type        = %s\n",  o, o->typeToString());
      creature->send("Object(%x)::wearable    = %s\n",  o, o->wearableToString());
      creature->send("Object(%x)::flags       = %lu\n", o, o->flags().value());
      creature->send("Object(%x)::shortname   = %s\n",  o, o->identifiers().shortname().c_str());
      creature->send("Object(%x)::longname    = %s\n",  o, o->identifiers().longname().c_str());
      creature->send("Object(%x)::description = %s\n",  o, o->identifiers().description().c_str());
      creature->send("Object(%x)::keywords    = %s\n",  o, o->identifiers().serializeKeywords().c_str());
      creature->send("Object(%x)::modifiers   = %s\n",  o, o->serializeModifiers().c_str());
      creature->send("Object(%x)::composition = %s\n",  o, o->serializeComposition().c_str());
      switch (o->type()) {
        case Object::Type_Furniture:
          creature->send("Furniture(%x)::capacity = %u\n", o, o->furniture()->capacity());
          creature->send("Furniture(%x)::layOn    = %u\n", o, o->furniture()->layOn());
          creature->send("Furniture(%x)::sitAt    = %u\n", o, o->furniture()->sitAt());
          creature->send("Furniture(%x)::sitOn    = %u\n", o, o->furniture()->sitOn());
          creature->send("Furniture(%x)::standOn  = %u\n", o, o->furniture()->standOn());
          break;
        case Object::Type_Weapon:
          creature->send("Weapon(%x)::type     = %s\n", o, o->weapon()->type().string().c_str());
          creature->send("Weapon(%x)::verb     = %s\n", o, o->weapon()->verb().string().c_str());
          creature->send("Weapon(%x)::damage   = %s\n", o, o->weapon()->damage().serialize().c_str());
          break;
        default:
          break;
      }
    }
  }
  fclose(fp);

  Mob* m = nullptr;
  std::map<unsigned long,Mob*>::iterator m_it;
  for (auto a_it : World::Instance().getAreas()) {
    if ((m_it = a_it->mobs().find(1002)) != a_it->mobs().end()) {
      m = Mob::create(m_it->second, creature->room());
    }
  }
  if (m == NULL) return false;
  fp = fopen("mob.txt", "w");
  Storage::dump(fp, m);
  fclose(fp);
  std::ifstream mob_ifs("mob.txt");
  std::string mob_content( (std::istreambuf_iterator<char>(mob_ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped Mob...\n");
  creature->send(mob_content);
  creature->send("\nLoaded Mob...\n");
  fp = fopen("mob.txt", "r");
  while (!feof(fp)) {
    m = new Mob();
    status = Storage::load(fp, m);
    if (status) {
      creature->send("Creature(%x)::gender          = %s\n", m, m->gender().string().c_str());
      creature->send("Creature(%x)::race            = %s\n", m, m->race().string().c_str());
      creature->send("Creature(%x)::pClass          = %s\n", m, m->pClass().string().c_str());
      creature->send("Creature(%x)::level           = %u\n", m, m->level());
      creature->send("Creature(%x)::exp             = %u\n", m, m->exp());
      creature->send("Creature(%x)::tnl             = %u\n", m, m->tnl());
      creature->send("Creature(%x)::health          = %d\n", m, m->health());
      creature->send("Creature(%x)::maxHealth       = %d\n", m, m->maxHealth());
      creature->send("Creature(%x)::mana            = %d\n", m, m->mana());
      creature->send("Creature(%x)::maxMana         = %d\n", m, m->maxMana());
      creature->send("Creature(%x)::stamina         = %d\n", m, m->stamina());
      creature->send("Creature(%x)::maxStrength     = %hu\n", m, m->maxStrength());
      creature->send("Creature(%x)::strength        = %hu\n", m, m->strength());
      creature->send("Creature(%x)::maxDexterity    = %hu\n", m, m->maxDexterity());
      creature->send("Creature(%x)::dexterity       = %hu\n", m, m->dexterity());
      creature->send("Creature(%x)::maxConstitution = %hu\n", m, m->maxConstitution());
      creature->send("Creature(%x)::constitution    = %hu\n", m, m->constitution());
      creature->send("Creature(%x)::maxIntelligence = %hu\n", m, m->maxIntelligence());
      creature->send("Creature(%x)::intelligence    = %hu\n", m, m->intelligence());
      creature->send("Creature(%x)::maxFocus        = %hu\n", m, m->maxFocus());
      creature->send("Creature(%x)::focus           = %hu\n", m, m->focus());
      creature->send("Creature(%x)::maxCreativity   = %hu\n", m, m->maxCreativity());
      creature->send("Creature(%x)::creativity      = %hu\n", m, m->creativity());
      creature->send("Creature(%x)::maxCharisma     = %hu\n", m, m->maxCharisma());
      creature->send("Creature(%x)::charisma        = %hu\n", m, m->charisma());
      creature->send("Creature(%x)::maxLuck         = %hu\n", m, m->maxLuck());
      creature->send("Creature(%x)::luck            = %hu\n", m, m->luck());
      creature->send("Creature(%x)::armor           = %d\n", m, m->armor());
      creature->send("Creature(%x)::bash            = %d\n", m, m->bash());
      creature->send("Creature(%x)::slash           = %d\n", m, m->slash());
      creature->send("Creature(%x)::pierce          = %d\n", m, m->pierce());
      creature->send("Creature(%x)::exotic          = %d\n", m, m->exotic());
      creature->send("Creature(%x)::trains          = %u\n", m, m->trains());
      creature->send("Creature(%x)::gold            = %u\n", m, m->gold());
      creature->send("Creature(%x)::silver          = %u\n", m, m->silver());
      creature->send("Mob(%x)::vnum           = %u\n", m, m->vnum());
      creature->send("Mob(%x)::mobility       = %u\n", m, m->mobility());
      creature->send("Mob(%x)::aggressiveness = %u\n", m, m->aggressiveness());
    }
  }
  fclose(fp);

  Avatar* av = nullptr;
  fp = fopen("avatar.txt", "w");
  Storage::dump(fp, (Avatar*)creature);
  fclose(fp);
  std::ifstream avatar_ifs("avatar.txt");
  std::string avatar_content( (std::istreambuf_iterator<char>(avatar_ifs)), (std::istreambuf_iterator<char>()) );
  creature->send("Dumped Avatar...\n");
  creature->send(avatar_content);
  creature->send("\nLoaded Avatar...\n");
  fp = fopen("avatar.txt", "r");
  while (!feof(fp)) {
    av = new Avatar();
    status = Storage::load(fp, av);
    if (status) {
      creature->send("Creature(%x)::gender          = %s\n", av, av->gender().string().c_str());
      creature->send("Creature(%x)::race            = %s\n", av, av->race().string().c_str());
      creature->send("Creature(%x)::pClass          = %s\n", av, av->pClass().string().c_str());
      creature->send("Creature(%x)::level           = %u\n", av, av->level());
      creature->send("Creature(%x)::exp             = %u\n", av, av->exp());
      creature->send("Creature(%x)::tnl             = %u\n", av, av->tnl());
      creature->send("Creature(%x)::health          = %d\n", av, av->health());
      creature->send("Creature(%x)::maxHealth       = %d\n", av, av->maxHealth());
      creature->send("Creature(%x)::mana            = %d\n", av, av->mana());
      creature->send("Creature(%x)::maxMana         = %d\n", av, av->maxMana());
      creature->send("Creature(%x)::stamina         = %d\n", av, av->stamina());
      creature->send("Creature(%x)::maxStrength     = %hu\n", av, av->maxStrength());
      creature->send("Creature(%x)::strength        = %hu\n", av, av->strength());
      creature->send("Creature(%x)::maxDexterity    = %hu\n", av, av->maxDexterity());
      creature->send("Creature(%x)::dexterity       = %hu\n", av, av->dexterity());
      creature->send("Creature(%x)::maxConstitution = %hu\n", av, av->maxConstitution());
      creature->send("Creature(%x)::constitution    = %hu\n", av, av->constitution());
      creature->send("Creature(%x)::maxIntelligence = %hu\n", av, av->maxIntelligence());
      creature->send("Creature(%x)::intelligence    = %hu\n", av, av->intelligence());
      creature->send("Creature(%x)::maxFocus        = %hu\n", av, av->maxFocus());
      creature->send("Creature(%x)::focus           = %hu\n", av, av->focus());
      creature->send("Creature(%x)::maxCreativity   = %hu\n", av, av->maxCreativity());
      creature->send("Creature(%x)::creativity      = %hu\n", av, av->creativity());
      creature->send("Creature(%x)::maxCharisma     = %hu\n", av, av->maxCharisma());
      creature->send("Creature(%x)::charisma        = %hu\n", av, av->charisma());
      creature->send("Creature(%x)::maxLuck         = %hu\n", av, av->maxLuck());
      creature->send("Creature(%x)::luck            = %hu\n", av, av->luck());
      creature->send("Creature(%x)::armor           = %d\n", av, av->armor());
      creature->send("Creature(%x)::bash            = %d\n", av, av->bash());
      creature->send("Creature(%x)::slash           = %d\n", av, av->slash());
      creature->send("Creature(%x)::pierce          = %d\n", av, av->pierce());
      creature->send("Creature(%x)::exotic          = %d\n", av, av->exotic());
      creature->send("Creature(%x)::trains          = %u\n", av, av->trains());
      creature->send("Creature(%x)::gold            = %u\n", av, av->gold());
      creature->send("Creature(%x)::silver          = %u\n", av, av->silver());
      creature->send("Avatar(%x)::gechoColor        = %c\n", av, av->gechoColor());
      creature->send("Avatar(%x)::title             = %s\n", av, av->title());
      creature->send("Avatar(%x)::poofin            = %s\n", av, av->poofin());
      creature->send("Avatar(%x)::poofout           = %s\n", av, av->poofout());
      creature->send("Avatar(%x)::age               = %hu\n", av, av->age());
      creature->send("Avatar(%x)::bankGold          = %lu\n", av, av->bankGold());
      creature->send("Avatar(%x)::bankSilver        = %lu\n", av, av->bankSilver());
      creature->send("Avatar(%x)::roomNumber        = %lu\n", av, av->roomNumber());
      creature->send("Avatar(%x)::adminFlags        = %lu\n", av, av->adminFlags().value());
      creature->send("Avatar(%x)::channelFlags      = %lu\n", av, av->channelFlags().value());
      creature->send("Avatar(%x)::whoFlags          = %lu\n", av, av->whoFlags().value());
    }
  }
  fclose(fp);

  // avatar()->send("No test code at the moment.");
  return true;
}
