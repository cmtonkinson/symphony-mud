
#include "dice.hpp"

Dice::Dice(void): _number(0), _faces(0) {
  return;
}

Dice::Dice(const Dice& ref): _number(ref.number()), _faces(ref.faces()) {
  return;
}

Dice::Dice(unsigned number, unsigned faces): _number(number), _faces(faces) {
  return;
}

Dice::~Dice(void) {
  return;
}

unsigned Dice::roll(void) const {
  unsigned total = 0;
  for (int i=0; i < number(); ++i) {
    total += rand() % faces() + 1;
  }
  return total;
}

unsigned Dice::average(void) const {
  double foo = (1.0 + (double)faces()) / 2.0; // average for one die
  return foo * (double)number();                // multiply by number of dice
}

const char* Dice::toString(void) const {
  std::string dest;
  dest.assign(estring(number())).append(1, 'd').append(estring(faces()));
  return dest.c_str();
}

std::string Dice::serialize(void) const {
  return toString();
}

void Dice::unserialize(std::string ser) {
  sscanf(ser.c_str(), "%ud%u", &_number, &_faces);
  return;
}
