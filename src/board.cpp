
#include "board.hpp"
#include "world.hpp"

Board::Board(void): _number(0) {
  return;
}

Board::Board(unsigned number) {
  this->number(number);
  return;
}

Board::~Board(void) {
  return;
}

const char* Board::name(void) const {
  switch (number()) {
    case GENERAL:   return "General";
    case CHANGES:   return "Changes";
    case ADMIN:     return "Admin";
    case NEWS:      return "News";
    case IMMORTAL:  return "Immortal";
    case BUILDING:  return "Building";
    default:        return "{RError{x";
  }
}

void Board::save(void) {
  for (auto it : notes()) it.second->save();
  return;
}
