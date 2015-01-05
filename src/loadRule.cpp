/*
 * $Id: loadRule.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "loadRule.h"
#include "world.h"



LoadRule::LoadRule( void ) {
  ID( 0 );
  vnum( 0 );
  room( NULL );
  type( 0 );
  target( 0 );
  number( 1 );
  max( 1 );
  probability( 100 );
  return;
}

LoadRule::LoadRule( ROW row ) {
  ID( row["id"] );
  vnum( row["vnum"] );
  room( World::Instance().findRoom( vnum() ) );
  if ( row["type"] == "MOB" ) {
    type( LOADRULE_MOB );
  } else if ( row["type"] == "OBJECT" ) {
    type( LOADRULE_OBJECT );
  }
  target( row["target"] );
  number( row["number"] );
  max( row["max"] );
  probability( row["probability"] );
  return;
}

LoadRule::~LoadRule( void ) {
  return;
}

bool LoadRule::destroy( void ) {
  unsigned long tempID = ID();

  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE              \
        FROM load_rules     \
        WHERE id = %lu      \
        LIMIT 1;",
      tempID
    );
    // Get rid of this (from the DB)...
    World::Instance().getMysql()->remove( query );
    // Get rid of this (from memory)...
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete load_rule %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  };

  // Success!
  return true;
}

