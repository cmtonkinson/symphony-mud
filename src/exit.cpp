/*
 * $Id: exit.cpp 417 2010-06-26 22:24:16Z cmtonkinson@gmail.com $
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

#include "exit.h"
#include "room.h"
#include "regex.h"
#include "world.h"

Exit::Exit( Room* target, ROW& row ):
    _flags( row["flags"] ) {
  this->target( target );
  ID( row["exitID"] );
  direction().set( row["direction"] );
  key( row["key"] );
  return;
}

Exit::Exit( Room* room, Room* target, const unsigned short& direction ) {
  try {
    char query[MAX_BUFFER];

    sprintf( query, "INSERT IGNORE INTO exits ( vnum, target, direction ) VALUES ( %lu, %lu, %u );", room->vnum(), target->vnum(), direction );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );

    this->target( target );
    this->direction().set( direction );
    flags().value( 0 );
    key( 0 );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create exit for room %lu: %s\n", room->ID(), me.getMessage().c_str() );
    return;
  }

  return;
}

Exit::~Exit( void ) {
  return;
}

void Exit::flag( const unsigned long& flag, const bool& value, bool stop ) {
  Exit* other = NULL;
  // Set the exit flag...
  _flags.set( flag, value );
  // If we find an opposite exit, also change that flag...
  if ( !stop ) { // this prevents recursion
    if ( ( other = target()->exit( inverse( direction().number() ) ) ) != NULL ) {
      other->flag( flag, value, true );
    }
  }
  return;
}

void Exit::save( void ) {
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " UPDATE exits SET      \
          flags = %lu,        \
          `key` = %lu         \
        WHERE exitID = %lu    \
        LIMIT 1;",
      flags().value(),
      key(),
      ID()
    );
    World::Instance().getMysql()->update( query );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save exit %lu: %s\n", ID(), me.getMessage().c_str() );
    return;
  }
  return;
}

void Exit::destroy( void ) {
  unsigned long tempID = ID();
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE              \
        FROM exits          \
        WHERE exitID = %lu  \
        LIMIT 1;",
      ID()
    );
    World::Instance().getMysql()->remove( query );
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete exit %lu: %s\n", tempID, me.getMessage().c_str() );
    return;
  }
  return;
}

unsigned short Exit::inverse( const unsigned short& direction ) { // static public
  switch ( direction ) {
    case NORTH: return SOUTH;
    case EAST:  return WEST;
    case SOUTH: return NORTH;
    case WEST:  return EAST;
    case UP:    return DOWN;
    case DOWN:  return UP;
    default:    return UNDEFINED;
  }
}

const char* Exit::name( const unsigned short& direction ) { // static public
  switch ( direction ) {
    case NORTH: return "North";
    case EAST:  return "East";
    case SOUTH: return "South";
    case WEST:  return "West";
    case UP:    return "Up";
    case DOWN:  return "Down";
    default:    return "error";
  }
}

const char* Exit::inverseName( const unsigned short& direction ) { // static public
  switch ( direction ) {
    case NORTH: return "South";
    case EAST:  return "West";
    case SOUTH: return "North";
    case WEST:  return "East";
    case UP:    return "Down";
    case DOWN:  return "Up";
    default:    return "error";
  }
}

unsigned short Exit::string2dir( const std::string& str ) { // static public
  std::string dir = Regex::trim(str);
  if ( Regex::strPrefix( str, "north" ) ) {
    return NORTH;
  } else if ( Regex::strPrefix( str, "east" ) ) {
    return EAST;
  } else if ( Regex::strPrefix( str, "south" ) ) {
    return SOUTH;
  } else if ( Regex::strPrefix( str, "west" ) ) {
    return WEST;
  } else if ( Regex::strPrefix( str, "up" ) ) {
    return UP;
  } else if ( Regex::strPrefix( str, "down" ) ) {
    return DOWN;
  } else {
    return UNDEFINED;
  }
}

unsigned short Exit::string2inverse( const std::string& str ) { // static public
  std::string dir = Regex::trim(str);
  if ( Regex::strPrefix( str, "north" ) ) {
    return SOUTH;
  } else if ( Regex::strPrefix( str, "east" ) ) {
    return WEST;
  } else if ( Regex::strPrefix( str, "south" ) ) {
    return NORTH;
  } else if ( Regex::strPrefix( str, "west" ) ) {
    return EAST;
  } else if ( Regex::strPrefix( str, "up" ) ) {
    return DOWN;
  } else if ( Regex::strPrefix( str, "down" ) ) {
    return UP;
  } else {
    return UNDEFINED;
  }
}
