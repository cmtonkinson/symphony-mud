/*
 * $Id: board.cpp 369 2010-05-31 15:48:57Z cmtonkinson@gmail.com $
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

#include "board.h"

#include "mysql.h"
#include "world.h"

Board::Board( const unsigned short& number ) {
  this->number( number );
  load();
  return;
}

Board::~Board( void ) {
  return;
}

const char* Board::name( void ) const {
  switch ( number() ) {
    case BOARD_GENERAL:   return "General";
    case BOARD_CHANGES:   return "Changes";
    case BOARD_ADMIN:     return "Admin";
    case BOARD_NEWS:      return "News";
    case BOARD_IMMORTAL:  return "Immortal";
    case BOARD_BUILDING:  return "Building";
    default:              return "{RError{x";
  }
}

bool Board::load( void ) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    ROW row;
    char query[MAX_BUFFER];
    sprintf( query, "SELECT * FROM notes WHERE board = %hu ORDER BY id ASC;", number() );
    if ( mysql->select( query ) ) {
      while ( (row = mysql->fetch()) ) {
        notes().insert( std::make_pair( row["id"], new Note( row ) ) );
      }
    }
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load notes for board %u: %s\n", number(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

void Board::save( void ) {
  for ( std::map<unsigned long,Note*>::iterator it = notes().begin(); it != notes().end(); ++it ) {
    it->second->save();
  }
  return;
}
