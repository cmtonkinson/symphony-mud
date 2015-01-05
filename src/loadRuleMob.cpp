/*
 * $Id: loadRuleMob.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#include "loadRuleMob.h"
#include "room.h"
#include "world.h"

LoadRuleMob::LoadRuleMob( void ) {
  return;
}

LoadRuleMob::LoadRuleMob( ROW row ): LoadRule( row ) {
  return;
}

LoadRuleMob::~LoadRuleMob( void ) {
  return;
}

std::string LoadRuleMob::notes( void ) const {
  return std::string();
}

bool LoadRuleMob::save( void ) {
  return false;
}

bool LoadRuleMob::commit( void ) {
  try {
    char query[MAX_BUFFER];

    sprintf(
      query,
      "INSERT IGNORE INTO load_rules ( vnum, type, target, number, max, probability ) VALUES ( %lu, '%s', %lu, %u, %u, %u );",
      vnum(),
      ( type() == LOADRULE_MOB ? "MOB" : "OBJECT" ),
      target(),
      number(),
      max(),
      probability()
    );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to commit load_rule vnum %lu: %s\n", vnum(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

bool LoadRuleMob::execute( std::list<Object*>& new_objects, std::list<Mob*>& new_mobs ) {
  std::map<unsigned long,Mob*>::iterator it;
  Area* area = room()->area();
  Mob* mob = NULL;
  unsigned already_there = area->howManyMobs( target() );
  unsigned mobs_added = 0;

  if ( ( it = area->mobs().find( target() ) ) != area->mobs().end() ) {
    mob = it->second;
  } else {
    World::Instance().bigBrother( NULL, ADMIN_BIGBRO_RESETS, "Failed to reset a mob.  Mob %lu in area %lu doesn't exist.", target(), area->ID() );
    return false;
  }

  for ( unsigned u = 0; u < number(); ++u ) {
    if ( already_there + mobs_added >= max() ) {
      break;
    }
    if ( World::rand( 1, 100 ) <= probability() ) {
      mob = new Mob( *mob );
      World::Instance().insert( mob );
      mob->room( room() );
      room()->add( mob );
      new_mobs.push_back( mob );
      mobs_added++;
    }
  }

  return true;
}
