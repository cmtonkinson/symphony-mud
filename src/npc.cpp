
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
  int level = ref.level();

  vnum(ref.vnum());
  mobility(ref.mobility());
  aggressiveness(ref.aggressiveness());
  formGroup();

  // Give the Npc some real stats. It's a hack to forcibly level them just to make the numbers
  // come out, but it works for now and it has the added benefit of making existing Npcs' stats
  // update to reflect modifications to the growth algorithms during active development.
  resetStats();
  for (int i = 1; i < level; ++i) gainLevel();

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

void Npc::_animate(void) {
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

  // Don't peace out while fighting.
  if (!opponents().empty()) return false;

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
  Npc* clone = new Npc(*npc);
  clone->room(room);
  return clone;
}

std::string Npc::printInformation(void) const {
  std::string dest;
  char buffer[Socket::MAX_BUFFER];

  sprintf(buffer, "\
        %s\n\
{w ____________________________________________________________________\n\
{w|\\__________________________________________________________________/|\n\
{w||{xrace:   {W%-9s{w ||{xstren: {C%2d{w ||{xhit: {R%-8d{w ||{xlevel: {Y%-10u{w ||\n\
{w||{xclass:  {W%-9s{w ||{xdexte: {C%2d{w ||{xdam: {R%-8d{w ||{xhealth: {G%4d{x/{g%-4d{w ||\n\
{w||{xgender: {W%-9s{w ||{xconst: {C%2d{w ||              ||{xmana: {C%4d{x/{c%-4d{w   ||\n\
{w||{xage:    {W%-9s{w ||{xintel: {C%2d{w ||{xarmor:  {B%-5d{w ||{xstamina: {M%-8d{w ||\n\
{w||{xhand:   {W%-9s{w ||{xfocus: {C%2d{w ||{xbash:   {b%-5d{w ||{xexp: {Y%-12u{w ||\n\
{w||{xheight: {W%-9s{w ||{xcreat: {C%2d{w ||{xslash:  {b%-5d{w ||{xtnl: {Y%-12u{w ||\n\
{w||{xweight: {W%-9s{w ||{xchari: {C%2d{w ||{xpierce: {b%-5d{w ||{xtrains: {B%-9u{w ||\n\
{w||{xtotem:  {W%-9s{w ||{xluck:  {C%2d{w ||{xexotic: {b%-5d{w ||                  ||\n\
{w||__________________||__________||______________||__________________||\n\
{w||__________________________________________________________________||\n\
{w||{xitems: {G%3zu{x/{g%-3zu{w ||{xcoins: {W%3u{x/{y%-4u{w    ||                            {w||\n\
{w||{xweight: {g%-5u{w  || {xbank: {W%-11s{w ||                            {w||\n\
{w||_______________||___________________||____________________________||\n\
{w|/__________________________________________________________________\\|{x\n\n",
    name(),
    race().string().c_str(), strength(), hitBonus(), level(),
    pClass().string().c_str(), dexterity(), damBonus(), health(), maxHealth(),
    gender().string().c_str(), constitution(), mana(), maxMana(),
    "N/A", intelligence(), armor(), stamina(),
    ((hand() == Being::WEARLOC_HOLD_R) ? "right" : "left"), focus(), bash(), exp(),
    "-", creativity(), slash(), tnl(),
    "-", charisma(), pierce(), trains(),
    "-", luck(), exotic(),
    inventory().itemList().size(), inventory().itemList().size(), money().silver(), money().gold(),
    0, "N/A"
  );
  dest.append(buffer);

  sprintf(buffer, "\
{w+----------------------+-------------------------------------+\n\
| {xvnum           {y%-5d{w | {xkeywords  {y%-25s{w |\n\
| {xmobility       {y%-5d{w | {xshortname {y%-25s{w |\n\
| {xaggressiveness {y%-5d{w | {xlongname  {y%-25s{w |\n\
+----------------------+-------------------------------------+{x",
    vnum(), identifiers().getKeywordList().c_str(),
    mobility(), identifiers().shortname().c_str(),
    aggressiveness(), identifiers().longname().c_str()
  );
  dest.append(buffer);

  return dest;
}
