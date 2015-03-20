
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

unsigned TerrainBeach::calculate(Being* being) {
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

unsigned TerrainCastle::calculate(Being* being) {
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

unsigned TerrainCave::calculate(Being* being) {
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

unsigned TerrainChurch::calculate(Being* being) {
  return base();
}

/************************************************************ CITY ************************************************************/
TerrainCity::TerrainCity(void) {
  name("city");
  return;
}

unsigned TerrainCity::calculate(Being* being) {
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

unsigned TerrainDeepwater::calculate(Being* being) {
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

unsigned TerrainDesert::calculate(Being* being) {
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

unsigned TerrainField::calculate(Being* being) {
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

unsigned TerrainForest::calculate(Being* being) {
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

unsigned TerrainGraveyard::calculate(Being* being) {
  return base();
}

/************************************************************ INDOOR ************************************************************/
TerrainIndoor::TerrainIndoor(void) {
  name("indoor");
  return;
}

unsigned TerrainIndoor::calculate(Being* being) {
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

unsigned TerrainJungle::calculate(Being* being) {
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

unsigned TerrainMine::calculate(Being* being) {
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

unsigned TerrainMountain::calculate(Being* being) {
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

unsigned TerrainPlains::calculate(Being* being) {
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

unsigned TerrainRoad::calculate(Being* being) {
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

unsigned TerrainSnow::calculate(Being* being) {
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

unsigned TerrainStone::calculate(Being* being) {
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

unsigned TerrainSwamp::calculate(Being* being) {
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

unsigned TerrainTreetop::calculate(Being* being) {
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

unsigned TerrainUnderwater::calculate(Being* being) {
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

unsigned TerrainVillage::calculate(Being* being) {
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

unsigned TerrainVolcano::calculate(Being* being) {
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

unsigned TerrainWater::calculate(Being* being) {
  return base();
}
