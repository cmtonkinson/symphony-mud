
#include "note.h"
#include "world.h"

Note::Note(const unsigned short& board, const unsigned long& author) {
  this->board(board);
  this->author(author);
  try {
    char query[Socket::MAX_BUFFER];
    sprintf(query, "INSERT IGNORE INTO notes (board, author) VALUES (%u, %lu);", this->board(), this->author());
    World::Instance().getMysql()->insert(query);
    ID(World::Instance().getMysql()->getInsertID());
  } catch (MysqlException me) {
    fprintf(stderr, "Failed to create note for board %u: %s\n", this->board(), me.getMessage().c_str());
    return;
  }
  return;
}

Note::Note(ROW row) {
  ID(row["id"]);
  board(row["board"]);
  author(row["author"]);
  subject(row["subject"]);
  body(row["body"]);
  return;
}

Note::~Note(void) {
  return;
}

bool Note::save(void) {
  try {
    char query[Socket::MAX_BUFFER];
    sprintf(query,
      "UPDATE notes SET subject = '%s', body = '%s' WHERE id = %lu LIMIT 1;",
      Mysql::addslashes(subject()).c_str(),
      Mysql::addslashes(body()).c_str(),
      ID()
   );
    World::Instance().getMysql()->update(query);
  } catch (MysqlException me) {
    fprintf(stderr, "Failed to save command %lu: %s\n", ID(), me.getMessage().c_str());
    return false;
  }
  return true;
}

bool Note::destroy(void) {
  unsigned long tempID = ID();
  try {
    char query[Socket::MAX_BUFFER];
    sprintf(query, "DELETE FROM notes WHERE id = %lu LIMIT 1;", ID());
    World::Instance().getMysql()->remove(query);
    delete this;
  } catch (MysqlException me) {
    fprintf(stderr, "Failed to delete note %lu: %s\n", tempID, me.getMessage().c_str());
    return false;
  }
  return true;
}
