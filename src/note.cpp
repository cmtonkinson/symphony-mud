
#include "note.hpp"
#include "world.hpp"

Note::Note(void) {
  return;
}

Note::Note(unsigned board_, std::string author_) {
  board(board_);
  author(author_);
  return;
}

Note::~Note(void) {
  return;
}

void Note::save(void) {
  return;
}

void Note::destroy(void) {
  delete this;
  return;
}
