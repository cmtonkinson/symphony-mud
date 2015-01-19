
#include "area.h"
#include "command.h"
#include "commandTable.h"
#include "io-handler.h"
#include "mob.h"
#include "mysql.h"
#include "world.h"

Mob::Mob(void): Creature() {
  return;
}

Mob::Mob(const Mob& ref): Creature(ref) {
  vnum(ref.vnum());
  formGroup();
  return;
}

Mob::Mob(ROW row): Creature() {
  ID(row["mobID"]);
  vnum(row["vnum"]);
  gender().set((unsigned)row["gender"]);
  race().set((unsigned)row["race"]);
  pClass().set((unsigned)row["pClass"]);
  identifiers().shortname(row["shortname"]);
  identifiers().longname(row["longname"]);
  identifiers().unserializeKeywords(row["keywords"]);
  identifiers().description(row["description"]);
  level(row["level"]);
  exp(row["exp"]);
  tnl(row["tnl"]);
  maxHealth(row["maxHealth"]);
  health(row["health"]);
  maxMana(row["maxMana"]);
  mana(row["mana"]);
  stamina(row["stamina"]);
  maxStrength(row["maxStrength"]);
  strength(row["strength"]);
  maxDexterity(row["maxDexterity"]);
  dexterity(row["dexterity"]);
  maxConstitution(row["maxConstitution"]);
  constitution(row["constitution"]);
  maxIntelligence(row["maxIntelligence"]);
  intelligence(row["intelligence"]);
  maxFocus(row["maxFocus"]);
  focus(row["focus"]);
  maxCreativity(row["maxCreativity"]);
  creativity(row["creativity"]);
  maxCharisma(row["maxCharisma"]);
  charisma(row["charisma"]);
  maxLuck(row["maxLuck"]);
  luck(row["luck"]);
  armor(row["armor"]);
  bash(row["bash"]);
  slash(row["slash"]);
  pierce(row["pierce"]);
  exotic(row["exotic"]);
  return;
}

Mob::Mob(Area* area, unsigned vnum): Creature() {

  try {
    char query[Socket::MAX_BUFFER];

    sprintf(query, "INSERT IGNORE INTO mobs (areaID, vnum) VALUES (%lu, %u);", area->ID(), vnum);
    World::Instance().getMysql()->insert(query);
    ID(World::Instance().getMysql()->getInsertID());

    area->mobs().insert(std::make_pair(vnum, this));
    this->vnum(vnum);
    level(1);

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to create mob for area %lu: %s\n", area->ID(), me.getMessage().c_str());
    return;
  }

  return;
}

Mob::~Mob(void) {
  return;
}

/******************************************************* Overloads of pure virtual methods ********************************************************/
bool Mob::save(void) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    char query[Socket::MAX_BUFFER];

    sprintf(query,
      "UPDATE mobs SET            \
        `gender` = %u ,           \
        `race` = %u ,             \
        `pClass` = %u ,           \
        `keywords` = '%s',        \
        `shortname` = '%s',       \
        `longname` = '%s',        \
        `description` = '%s',     \
        `level` = %u,             \
        `exp` = %u,               \
        `tnl` = %u,               \
        `health` = %d,            \
        `maxHealth` = %d,         \
        `mana` = %d,              \
        `maxMana` = %d,           \
        `stamina` = %d,           \
        `strength` = %hu,         \
        `maxStrength` = %hu,      \
        `dexterity` = %hu,        \
        `maxDexterity` = %hu,     \
        `constitution` = %hu,     \
        `maxConstitution` = %hu,  \
        `intelligence` = %hu,     \
        `maxIntelligence` = %hu,  \
        `focus` = %hu,            \
        `maxFocus` = %hu,         \
        `creativity` = %hu,       \
        `maxCreativity` = %hu,    \
        `charisma` = %hu,         \
        `maxCharisma` = %hu,      \
        `luck` = %hu,             \
        `maxLuck` = %hu,          \
        `armor` = %d,             \
        `bash` = %d,              \
        `slash` = %d,             \
        `pierce` = %d,            \
        `exotic` = %d             \
       WHERE mobID = %lu          \
       LIMIT 1;",
      gender().number(),
      race().number(),
      pClass().number(),
      Mysql::addslashes(identifiers().serializeKeywords()).c_str(),
      Mysql::addslashes(identifiers().shortname()).c_str(),
      Mysql::addslashes(identifiers().longname()).c_str(),
      Mysql::addslashes(identifiers().description()).c_str(),
      level(),
      exp(),
      tnl(),
      health(),
      maxHealth(),
      mana(),
      maxMana(),
      stamina(),
      strength(),
      maxStrength(),
      dexterity(),
      maxDexterity(),
      constitution(),
      maxConstitution(),
      intelligence(),
      maxIntelligence(),
      focus(),
      maxFocus(),
      creativity(),
      maxCreativity(),
      charisma(),
      maxCharisma(),
      luck(),
      maxLuck(),
      armor(),
      bash(),
      slash(),
      pierce(),
      exotic(),
      ID()
   );
    mysql->update(query);

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to save mob %lu: %s\n", ID(), me.getMessage().c_str());
    return false;
  }

  return true;
}

bool Mob::destroy(void) {
  unsigned long tempID = ID();
  try {
    char query[Socket::MAX_BUFFER];

    sprintf(query,
      " DELETE                \
        FROM mobs             \
        WHERE mobID = %lu     \
        LIMIT 1;",
      ID()
   );
    World::Instance().getMysql()->remove(query);
    delete this;

  } catch (MysqlException me) {
    fprintf(stderr, "Failed to delete mob %lu: %s\n", tempID, me.getMessage().c_str());
    return false;
  }
  return true;
}

/******************************************************* Overloads of virtual methods ********************************************************/
void Mob::whatHappensWhenIDie(void) {
  ungroup();
  room()->remove(this);
  World::Instance().remove(this);
  delete this;
  return;
}

/******************************************************* Static methods ********************************************************/
Mob* Mob::create(Area* area, unsigned vnum) {
  return new Mob(area, vnum);
}

Mob* Mob::create(ROW row) {
  return new Mob(row);
}

Mob* Mob::create(Mob* mob, Room* room) {
  Mob* m = new Mob(*mob);
  m->room(room);
  return m;
}

std::string Mob::getInformation(Mob* mob) {
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  output.append("  --== {Ybasic mob data{x ==--\n");
  // Basic mob information...
  sprintf(buffer, "vnum......... {y%u{x\n\
level........ {y%u{x\n\
race......... {y%s{x\n\
class........ {y%s{x\n\
gender....... {y%s{x\n\
keywords..... {y%s{x\n\
shortname.... %s\n\
longname..... %s\n\n\
  --== {Y description{x ==--\n%s\n\
",  mob->vnum(), mob->level(), mob->race().string().c_str(), mob->pClass().string().c_str(), mob->gender().string().c_str(),
    mob->identifiers().getKeywordList().c_str(), mob->identifiers().shortname().c_str(), mob->identifiers().longname().c_str(),
    mob->identifiers().description().c_str()
 );
  output.append(buffer);
  output.append("  --== {Ystats{x ==--\n");
  sprintf(buffer, "health....... {G%d{x/{g%d{x\n\
mana......... {C%d{x/{c%d{x\n\
stamina...... {M%d{x\n",
    mob->health(), mob->maxHealth(),
    mob->mana(), mob->maxMana(),
    mob->stamina()
 );
  output.append(buffer);

  return output;
}
