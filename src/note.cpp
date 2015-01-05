/*
 * $Id: note.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "note.h"
#include "world.h"

Note::Note( const unsigned short& board, const unsigned long& author ) {
  this->board( board );
  this->author( author );
  try {
    char query[MAX_BUFFER];
    sprintf( query, "INSERT IGNORE INTO notes ( board, author ) VALUES ( %u, %lu );", this->board(), this->author() );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create note for board %u: %s\n", this->board(), me.getMessage().c_str() );
    return;
  }
  return;
}

Note::Note( ROW row ) {
  ID( row["id"] );
  board( row["board"] );
  author( row["author"] );
  subject( row["subject"] );
  body( row["body"] );
  return;
}

Note::~Note( void ) {
  return;
}

bool Note::save( void ) {
  try {
    char query[MAX_BUFFER];
    sprintf( query,
      "UPDATE notes SET subject = '%s', body = '%s' WHERE id = %lu LIMIT 1;",
      Mysql::addslashes(subject()).c_str(),
      Mysql::addslashes(body()).c_str(),
      ID()
    );
    World::Instance().getMysql()->update( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save command %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }
  return true;
}

bool Note::destroy( void ) {
  unsigned long tempID = ID();
  try {
    char query[MAX_BUFFER];
    sprintf( query, "DELETE FROM notes WHERE id = %lu LIMIT 1;", ID() );
    World::Instance().getMysql()->remove( query );
    delete this;
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete note %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  }
  return true;
}
