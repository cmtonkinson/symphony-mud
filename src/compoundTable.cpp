
#include "compoundTable.h"
#include "display.h"

CompoundTable::CompoundTable(void) {
  /* Attributes NOT of the "arbitrarily relative 0-1 scale" nature are marked with an "X" for ease of reference.
   *                                                        X                    X                          X                           X                       X
   *                 short,       long,           dark, density, duct, durab, elCond, flamm, flexi, fragr, hard, lustr, magic, malea, melting, react, smooth, thCond   */
  // elemental metal
  add(new Compound("aluminum",  "aluminum",      0.0,   2.700,  0.0,   0.0,  3.770,   0.0,   0.0,   0.0, 2.75,   0.0,   0.0,   0.0,  933.47,   0.0,    0.0,  237.0, COMPOUND_METAL));
  add(new Compound("copper",    "copper",        0.0,   8.960,  0.0,   0.0,  5.960,   0.0,   0.0,   0.0, 3.00,   0.0,   0.0,   0.0, 1356.15,   0.0,    0.0,  401.0, COMPOUND_METAL));
  add(new Compound("gold",      "gold",          0.0,  19.299,  0.0,   0.0,  4.520,   0.0,   0.0,   0.0, 2.50,   0.0,   0.0,   0.0, 1337.33,   0.0,    0.0,  317.0, COMPOUND_METAL));
  add(new Compound("iron",      "iron",          0.0,   7.874,  0.0,   0.0,  0.933,   0.0,   0.0,   0.0, 4.00,   0.0,   0.0,   0.0, 1811.00,   0.0,    0.0,   80.2, COMPOUND_METAL));
  add(new Compound("platinum",  "platinum",      0.0,  21.400,  0.0,   0.0,  0.966,   0.0,   0.0,   0.0, 4.25,   0.0,   0.0,   0.0, 2041.40,   0.0,    0.0,   71.6, COMPOUND_METAL));
  add(new Compound("lead",      "lead",          0.0,  11.400,  0.0,   0.0,  0.481,   0.0,   0.0,   0.0, 1.50,   0.0,   0.0,   0.0,  600.61,   0.0,    0.0,    3.5, COMPOUND_METAL));
  add(new Compound("silver",    "silver",        0.0,  10.500,  0.0,   0.0,  6.300,   0.0,   0.0,   0.0, 2.50,   0.0,   0.0,   0.0, 1234.93,   0.0,    0.0,  429.0, COMPOUND_METAL));
  add(new Compound("tin",       "tin",           0.0,   7.300,  0.0,   0.0,  0.917,   0.0,   0.0,   0.0, 1.50,   0.0,   0.0,   0.0,  505.08,   0.0,    0.0,   66.6, COMPOUND_METAL));
  add(new Compound("zinc",      "zinc",          0.0,   7.140,  0.0,   0.0,  1.660,   0.0,   0.0,   0.0, 2.50,   0.0,   0.0,   0.0,  692.68,   0.0,    0.0,  116.0, COMPOUND_METAL));
  add(new Compound("mythril",   "mythril",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_METAL));
  // alloy
  // stone
  add(new Compound("amethyst",  "amethyst",      0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("crystal",   "crystal",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE));
  add(new Compound("diamond",   "diamond",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("emerald",   "emerald",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("jade",      "jade",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("onyx",      "onyx",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE));
  add(new Compound("quartz",    "quartz",        0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE));
  add(new Compound("ruby",      "ruby",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("sapphire",  "sapphire",      0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE | COMPOUND_GEM));
  add(new Compound("stone",     "stone",         0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_STONE));
  // wood
  add(new Compound("ash",       "ash wood",      0.0,   0.600,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("beech",     "beech wood",    0.0,   0.800,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("birch",     "birch wood",    0.0,   0.670,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("cedar",     "cedar wood",    0.0,   0.380,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("cherry",    "cherry wood",   0.0,   0.800,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("ironwood",  "ironwood",      0.0,   1.250,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("maple",     "maple wood",    0.0,   0.650,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("oak",       "oak wood",      0.0,   0.750,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("pine",      "pine wood",     0.0,   0.450,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("walnut",    "walnut wood",   0.0,   0.550,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  add(new Compound("yew",       "yew wood",      0.0,   0.600,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_WOOD));
  // skin
  add(new Compound("bear",      "bear skin",     0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("deer",      "deer skin",     0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("dragon",    "dragon skin",   0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("rabbit",    "rabbit skin",   0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("raccoon",   "raccoon skin",  0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("snake",     "snake skin",    0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  add(new Compound("wolf",      "wolf skin",     0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_SKIN));
  // cloth
  add(new Compound("burlap",    "burlap",        0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  add(new Compound("canvas",    "canvas",        0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  add(new Compound("cotton",    "cotton",        0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  add(new Compound("leather",   "leather",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  add(new Compound("silk",      "silk",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  add(new Compound("wool",      "wool",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, COMPOUND_CLOTH));
  // other
  add(new Compound("bone",      "bone",          0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, 0));
  add(new Compound("feather",   "feather",       0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, 0));
  add(new Compound("glass",     "glass",         0.0,   0.000,  0.0,   0.0,  0.000,   0.0,   0.0,   0.0, 0.00,   0.0,   0.0,   0.0,      0.00, 0.0,   0.0,     0.0, 0));
  return;
}

CompoundTable::~CompoundTable(void) {
  return;
}

void CompoundTable::add(Compound* t) {
  compounds().insert(std::make_pair(t->identifiers().shortname(), t));
  return;
}

Compound* CompoundTable::find(const std::string& name) const {
  std::map<std::string,Compound*>::const_iterator it;
  if ((it = compounds().find(name)) != compounds().end()) {
    return it->second;
  }
  for (it = compounds().begin(); it != compounds().end(); ++it) {
    if (Regex::strPrefix(name, it->second->identifiers().shortname())) {
      return it->second;
    }
  }
  return NULL;
}

std::string CompoundTable::list(std::string glue) const {
  std::vector<std::string> items;
  for (std::map<std::string,Compound*>::const_iterator it = compounds().begin(); it != compounds().end(); ++it) {
    items.push_back(it->second->identifiers().longname());
  }
  return Display::formatColumns(items);
}
