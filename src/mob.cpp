
#include <math.h>
#include "area.h"
#include "command.h"
#include "commandTable.h"
#include "io-handler.h"
#include "mob.h"
#include "world.h"

// Static constants need external linkage.
const unsigned Mob::MIN_MOBILITY;
const unsigned Mob::DEFAULT_MOBILITY;
const unsigned Mob::MAX_MOBILITY;
const unsigned Mob::MIN_AGGRESSIVENESS;
const unsigned Mob::DEFAULT_AGGRESSIVENESS;
const unsigned Mob::MAX_AGGRESSIVENESS;

Mob::Mob(void): Creature() {
  mobility(DEFAULT_MOBILITY);
  aggressiveness(DEFAULT_AGGRESSIVENESS);
  return;
}

Mob::Mob(const Mob& ref): Creature(ref) {
  vnum(ref.vnum());
  mobility(ref.mobility());
  aggressiveness(ref.aggressiveness());
  formGroup();
  return;
}

Mob::Mob(Area* area, unsigned vnum): Mob() {

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

void Mob::mobility(unsigned mobility) {
  _mobility = std::max(MIN_MOBILITY, std::min(MAX_MOBILITY, mobility));
  return;
}

void Mob::aggressiveness(unsigned aggressiveness) {
  _aggressiveness = std::max(MIN_AGGRESSIVENESS, std::min(MAX_AGGRESSIVENESS, aggressiveness));
  return;
}

void Mob::whatHappensWhenIDie(void) {
  ungroup();
  room()->remove(this);
  World::Instance().remove(this);
  delete this;
  return;
}

void Mob::mobilize(void) {
  double base        = 0.0;
  double smudge      = 0.0;
  double lower_bound = 0.0;
  double upper_bound = 0.0;
  // Movement equation: 3^(7-x)
  // For each (x,y) the average movement time (2, 243), (3, 81), (4, 27), (5, 9)
  base        = pow(3, 7 - mobility());
  smudge      = 0.25;
  lower_bound = base * (1 - smudge);
  upper_bound = base * (1 + smudge);
  World::Instance().schedule()->add(new RecurringJob(this, &Mob::auto_move, lower_bound, upper_bound));
  return;
}

bool Mob::auto_move(Job* job) {
  unsigned directions[]     = { NORTH, EAST, SOUTH, WEST, UP, DOWN };
  unsigned chosen_direction = 0;
  std::string foo;
  std::vector<unsigned> valid_exits;

  // Enumerate the possible exits.
  for (auto iter : directions) {
    if (canMove(iter, foo)) {
      valid_exits.push_back(iter);
    }
  }

  // Bail if there's no way out.
  if (valid_exits.empty()) return false;

  // Pick a direction and go!
  chosen_direction = valid_exits[World::rand(0, valid_exits.size() - 1)];
  return move(chosen_direction);
}

Mob* Mob::create(Area* area, unsigned vnum) {
  return new Mob(area, vnum);
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
  sprintf(buffer, "health......... {G%d{x/{g%d{x\n\
mana............ {C%d{x/{c%d{x\n\
stamina......... {M%d{x\n\
mobility........ {B%u{x\n\
aggressiveness.. {R%u{x\n",
    mob->health(), mob->maxHealth(),
    mob->mana(), mob->maxMana(),
    mob->stamina(),
    mob->mobility(),
    mob->aggressiveness()
 );
  output.append(buffer);

  return output;
}
