
#include <math.h>
#include "command.hpp"
#include "command-table.hpp"
#include "io-handler.hpp"
#include "math.hpp"
#include "npc.hpp"
#include "world.hpp"
#include "zone.hpp"

// Static constants need external linkage.
const unsigned Npc::MIN_NPCILITY;
const unsigned Npc::DEFAULT_NPCILITY;
const unsigned Npc::MAX_NPCILITY;
const unsigned Npc::MIN_AGGRESSIVENESS;
const unsigned Npc::DEFAULT_AGGRESSIVENESS;
const unsigned Npc::MAX_AGGRESSIVENESS;

Npc::Npc(void): Being() {
  mobility(DEFAULT_NPCILITY);
  aggressiveness(DEFAULT_AGGRESSIVENESS);
  return;
}

Npc::Npc(const Npc& ref): Being(ref) {
  vnum(ref.vnum());
  mobility(ref.mobility());
  aggressiveness(ref.aggressiveness());
  formGroup();
  return;
}

Npc::Npc(Zone* zone, unsigned vnum_): Npc() {
  zone->npcs().insert(std::make_pair(vnum_, this));
  vnum(vnum_);
  level(1);
  return;
}

Npc::~Npc(void) {
  return;
}

void Npc::mobility(unsigned mobility) {
  _mobility = std::max(MIN_NPCILITY, std::min(MAX_NPCILITY, mobility));
  return;
}

void Npc::aggressiveness(unsigned aggressiveness) {
  _aggressiveness = std::max(MIN_AGGRESSIVENESS, std::min(MAX_AGGRESSIVENESS, aggressiveness));
  return;
}

void Npc::whatHappensWhenIDie(void) {
  ungroup();
  room()->remove(this);
  World::Instance().remove(this);
  delete this;
  return;
}

void Npc::mobilize(void) {
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
  World::Instance().schedule()->add(new RecurringJob(this, &Npc::auto_move, "Npc::auto_move", lower_bound, upper_bound));
  return;
}

bool Npc::auto_move(Job* job) {
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
  chosen_direction = valid_exits[Math::rand(0, valid_exits.size() - 1)];
  return move(chosen_direction);
}

Npc* Npc::create(Zone* zone, unsigned vnum) {
  return new Npc(zone, vnum);
}

Npc* Npc::create(Npc* npc, Room* room) {
  Npc* m = new Npc(*npc);
  m->room(room);
  return m;
}

std::string Npc::getInformation(Npc* npc) {
  std::string output;
  char buffer[Socket::MAX_BUFFER];

  output.append("  --== {Ybasic npc data{x ==--\n");
  // Basic npc information...
  sprintf(buffer, "vnum......... {y%u{x\n\
level........ {y%u{x\n\
race......... {y%s{x\n\
class........ {y%s{x\n\
gender....... {y%s{x\n\
keywords..... {y%s{x\n\
shortname.... %s\n\
longname..... %s\n\n\
  --== {Y description{x ==--\n%s\n\
",  npc->vnum(), npc->level(), npc->race().string().c_str(), npc->pClass().string().c_str(), npc->gender().string().c_str(),
    npc->identifiers().getKeywordList().c_str(), npc->identifiers().shortname().c_str(), npc->identifiers().longname().c_str(),
    npc->identifiers().description().c_str()
 );
  output.append(buffer);
  output.append("  --== {Ystats{x ==--\n");
  sprintf(buffer, "health......... {G%d{x/{g%d{x\n\
mana............ {C%d{x/{c%d{x\n\
stamina......... {M%d{x\n\
mobility........ {B%u{x\n\
aggressiveness.. {R%u{x\n",
    npc->health(), npc->maxHealth(),
    npc->mana(), npc->maxMana(),
    npc->stamina(),
    npc->mobility(),
    npc->aggressiveness()
 );
  output.append(buffer);

  return output;
}
