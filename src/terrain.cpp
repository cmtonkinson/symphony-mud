/*
 * $Id: terrain.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
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

#include "terrain.h"

/************************************************************ BASE CLASS ************************************************************/
Terrain::Terrain(void) {
  base(DEFAULT_MOVEMENT);
  title('g');
  description('x');
  map('x');
  flags(0);
  return;
}

Terrain::~Terrain(void) {
  return;
}

/************************************************************ BEACH ************************************************************/
TerrainBeach::TerrainBeach(void) {
  name("beach");
  title('y');
  description('Y');
  map('Y');
  return;
}

unsigned short TerrainBeach::calculate(Creature* creature) {
  return base();
}

/************************************************************ CASTLE ************************************************************/
TerrainCastle::TerrainCastle(void) {
  name("castle");
  title('w');
  description('X');
  map('w');
  return;
}

unsigned short TerrainCastle::calculate(Creature* creature) {
  return base();
}

/************************************************************ CAVE ************************************************************/
TerrainCave::TerrainCave(void) {
  name("cave");
  title('W');
  description('x');
  map('w');
  return;
}

unsigned short TerrainCave::calculate(Creature* creature) {
  return base();
}

/************************************************************ CHURCH ************************************************************/
TerrainChurch::TerrainChurch(void) {
  name("church");
  title('w');
  description('W');
  map('w');
  return;
}

unsigned short TerrainChurch::calculate(Creature* creature) {
  return base();
}

/************************************************************ CITY ************************************************************/
TerrainCity::TerrainCity(void) {
  name("city");
  return;
}

unsigned short TerrainCity::calculate(Creature* creature) {
  return base();
}

/************************************************************ DEEPWATER ************************************************************/
TerrainDeepwater::TerrainDeepwater(void) {
  name("deep_water");
  title('C');
  description('b');
  map('B');
  return;
}

unsigned short TerrainDeepwater::calculate(Creature* creature) {
  return base();
}

/************************************************************ DESERT ************************************************************/
TerrainDesert::TerrainDesert(void) {
  name("desert");
  title('Y');
  description('y');
  map('y');
  return;
}

unsigned short TerrainDesert::calculate(Creature* creature) {
  return base();
}

/************************************************************ FIELD ************************************************************/
TerrainField::TerrainField(void) {
  name("field");
  title('g');
  description('G');
  map('G');
  return;
}

unsigned short TerrainField::calculate(Creature* creature) {
  return base();
}

/************************************************************ FOREST ************************************************************/
TerrainForest::TerrainForest(void) {
  name("forest");
  title('y');
  description('g');
  map('g');
  return;
}

unsigned short TerrainForest::calculate(Creature* creature) {
  return base();
}

/************************************************************ GRAVEYARD ************************************************************/
TerrainGraveyard::TerrainGraveyard(void) {
  name("graveyard");
  title('W');
  description('w');
  map('w');
  return;
}

unsigned short TerrainGraveyard::calculate(Creature* creature) {
  return base();
}

/************************************************************ INDOOR ************************************************************/
TerrainIndoor::TerrainIndoor(void) {
  name("indoor");
  return;
}

unsigned short TerrainIndoor::calculate(Creature* creature) {
  return base();
}

/************************************************************ JUNGLE ************************************************************/
TerrainJungle::TerrainJungle(void) {
  name("jungle");
  title('G');
  description('g');
  map('g');
  return;
}

unsigned short TerrainJungle::calculate(Creature* creature) {
  return base();
}

/************************************************************ MINE ************************************************************/
TerrainMine::TerrainMine(void) {
  name("mine");
  title('w');
  description('m');
  map('m');
  return;
}

unsigned short TerrainMine::calculate(Creature* creature) {
  return base();
}

/************************************************************ MOUNTAIN ************************************************************/
TerrainMountain::TerrainMountain(void) {
  name("mountain");
  title('m');
  description('W');
  map('m');
  return;
}

unsigned short TerrainMountain::calculate(Creature* creature) {
  return base();
}

/************************************************************ PLAINS ************************************************************/
TerrainPlains::TerrainPlains(void) {
  name("plains");
  title('Y');
  description('G');
  map('G');
  return;
}

unsigned short TerrainPlains::calculate(Creature* creature) {
  return base();
}

/************************************************************ ROAD ************************************************************/
TerrainRoad::TerrainRoad(void) {
  name("road");
  title('g');
  description('y');
  map('x');
  return;
}

unsigned short TerrainRoad::calculate(Creature* creature) {
  return base();
}

/************************************************************ SNOW ************************************************************/
TerrainSnow::TerrainSnow(void) {
  name("snow");
  title('x');
  description('W');
  map('W');
  return;
}

unsigned short TerrainSnow::calculate(Creature* creature) {
  return base();
}

/************************************************************ STONE ************************************************************/
TerrainStone::TerrainStone(void) {
  name("stone");
  title('w');
  description('x');
  map('x');
  return;
}

unsigned short TerrainStone::calculate(Creature* creature) {
  return base();
}

/************************************************************ SWAMP ************************************************************/
TerrainSwamp::TerrainSwamp(void) {
  name("swamp");
  title('y');
  description('c');
  map('c');
  return;
}

unsigned short TerrainSwamp::calculate(Creature* creature) {
  return base();
}

/************************************************************ TREETOP ************************************************************/
TerrainTreetop::TerrainTreetop(void) {
  name("treetop");
  title('y');
  description('g');
  map('g');
  return;
}

unsigned short TerrainTreetop::calculate(Creature* creature) {
  return base();
}

/************************************************************ UNDERWATER ************************************************************/
TerrainUnderwater::TerrainUnderwater(void) {
  name("underwater");
  title('C');
  description('c');
  map('b');
  return;
}

unsigned short TerrainUnderwater::calculate(Creature* creature) {
  return base();
}

/************************************************************ VILLAGE ************************************************************/
TerrainVillage::TerrainVillage(void) {
  name("village");
  title('G');
  description('y');
  map('x');
  return;
}

unsigned short TerrainVillage::calculate(Creature* creature) {
  return base();
}

/************************************************************ VOLCANO ************************************************************/
TerrainVolcano::TerrainVolcano(void) {
  name("volcano");
  title('w');
  description('R');
  map('R');
  return;
}

unsigned short TerrainVolcano::calculate(Creature* creature) {
  return base();
}

/************************************************************ WATER ************************************************************/
TerrainWater::TerrainWater(void) {
  name("water");
  title('C');
  description('B');
  map('C');
  return;
}

unsigned short TerrainWater::calculate(Creature* creature) {
  return base();
}

