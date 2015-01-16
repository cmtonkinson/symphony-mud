
#include "board.h"
#include "mysql.h"
#include "world.h"

Board::Board(const unsigned short& number) {
  this->number(number);
  load();
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
    default:              return "{RError{x";
  }
}

bool Board::load(void) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    ROW row;
    char query[Socket::MAX_BUFFER];
    sprintf(query, "SELECT * FROM notes WHERE board = %hu ORDER BY id ASC;", number());
    if (mysql->select(query)) {
      while ((row = mysql->fetch())) {
        notes().insert(std::make_pair(row["id"], new Note(row)));
      }
    }
  } catch (MysqlException me) {
    fprintf(stderr, "Failed to load notes for board %u: %s\n", number(), me.getMessage().c_str());
    return false;
  }

  return true;
}

void Board::save(void) {
  for (std::map<unsigned long,Note*>::iterator it = notes().begin(); it != notes().end(); ++it) {
    it->second->save();
  }
  return;
}
