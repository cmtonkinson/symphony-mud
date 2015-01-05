/*
 * $Id: area.cpp 457 2010-08-02 19:25:53Z cmtonkinson@gmail.com $
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

#include "area.h"
#include "exit.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "room.h"
#include "terrainTable.h"
#include "world.h"

Area::Area( void ) {
  return;
}

Area::Area( const unsigned long& low, const unsigned long& high ) {
  name( "Undefined" );
  this->low( low );
  this->high( high );
  terrain( TerrainTable::Instance().find( "city" ) );
  create();
}

Area::~Area( void ) {
  return;
}

void Area::name( const std::string& name ) {
  ColorString cs( name );
  _name = cs.stripColor();
  return;
}

void Area::create( void ) {
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      "INSERT INTO areas    \
        ( low, high, name ) \
        VALUES              \
        ( %lu, %lu, '%s' ); \
      ;",
      low(),
      high(),
      Mysql::addslashes(name()).c_str()
    );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create area: %s\n", me.getMessage().c_str() );
    return;
  }

  return;
}

bool Area::load( ROW& row ) {
  ID( row["areaID"] );
  low( row["low"] );
  high( row["high"] );
  name( row["name"] );
  terrain( TerrainTable::Instance().find( row["terrain"] ) );
  World::Instance().schedule()->add( new RecurringJob( this, &Area::reset, 300, 600 ) );
  return true;
}

bool Area::loadRooms( void ) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    ROW row;
    char query[MAX_BUFFER];

    sprintf( query, "SELECT * FROM rooms WHERE areaID = %lu ORDER BY vnum ASC;", ID() );
    if ( mysql->select( query ) ) {
      while ( row = mysql->fetch() ) {
        rooms().insert( std::make_pair( row["vnum"], new Room( this, row ) ) );
      }
    }

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load rooms for area %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }

  try {
    Mysql* mysql = World::Instance().getMysql();
    ROW row;
    char query[MAX_BUFFER];

    for ( std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it ) {
      sprintf( query, "SELECT * FROM load_rules WHERE vnum = %lu ORDER BY id ASC;", it->second->vnum() );
      if ( mysql->select( query ) ) {
        while ( row = mysql->fetch() ) {
          if ( row["type"] == "MOB" ) {
            it->second->add( new LoadRuleMob( row ) );
          } else if ( row["type"] == "OBJECT" ) {
            it->second->add( new LoadRuleObject( row ) );
          } else {
            fprintf( stderr, "Funky load rule id %lu\n", (unsigned long)estring(row["id"]) );
          }
        }
      }
    }

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load Rules for area %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

bool Area::loadExits( void ) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    Room* target = NULL;
    ROW row;
    char query[MAX_BUFFER];
    for ( std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it ) {
      sprintf( query, "SELECT * FROM exits WHERE vnum = %lu LIMIT 6;", it->second->vnum() );
      if ( mysql->select( query ) ) {
        while ( row = mysql->fetch() ) {
          if ( ( target = World::Instance().findRoom( row["target"] ) ) != NULL ) {
            it->second->exit( row["direction"], new Exit( target, row ) );
          }
        }
      }
    }
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load exits for area %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

void Area::loadObjects( Mysql* db ) {
  ROW row;
  char query[MAX_BUFFER];
  sprintf( query, "SELECT * FROM objects WHERE areaID = %lu ORDER BY vnum ASC;", ID() );
  if ( db->select( query ) ) {
    while ( row = db->fetch() ) {
      objects().insert( std::make_pair( row["vnum"], new Object( row ) ) );
    }
  }
  return;
}

bool Area::loadMobs( void ) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    ROW row;
    char query[MAX_BUFFER];

    sprintf( query, "SELECT * FROM mobs WHERE areaID = %lu ORDER BY vnum ASC;", ID() );
    if ( mysql->select( query ) ) {
      while ( row = mysql->fetch() ) {
        mobs().insert( std::make_pair( row["vnum"], Mob::create( row ) ) );
      }
    }

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load mobs for area %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

void Area::save( Mysql* db ) {
  char query[MAX_BUFFER];

  sprintf( query,
    "UPDATE areas SET low = %lu, high = %lu, name = '%s', terrain = '%s' WHERE areaID = %lu LIMIT 1;",
    low(), high(), Mysql::addslashes(name()).c_str(), Mysql::addslashes(terrain()->name()).c_str(), ID()
  );
  db->update( query );

  // Save rooms...
  for ( std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it ) {
    it->second->save();
  }

  // Save objects...
  for ( std::map<unsigned long,Object*>::iterator it = objects().begin(); it != objects().end(); ++it ) {
    it->second->update( db );
  }

  // Save mobs...
  for ( std::map<unsigned long,Mob*>::iterator it = mobs().begin(); it != mobs().end(); ++it ) {
    it->second->save();
  }

  return;
}

bool Area::destroy( Mysql* db ) {
  Object* object = NULL;
  unsigned long tempID = ID();

  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE              \
        FROM areas          \
        WHERE areaID = %lu  \
        LIMIT 1;",
      ID()
    );
    // Get rid of this (from the DB)...
    World::Instance().getMysql()->remove( query );
    // Get rid of this (from the world)...
    World::Instance().remove( this );
    // Get rid of rooms...
    while ( rooms().size() ) {
      if ( !rooms().begin()->second->destroy() ) {
        World::worldLog( ERROR, LOG_WORLD, "Room %lu failed to destroy itself with area %lu (%s).", rooms().begin()->second->vnum(), ID(), name().c_str() );
        return false;
      }
    }
    // Get rid of objects...
    while ( objects().size() ) {
      object = objects().begin()->second;
      try {
        object->destroy( db );
      } catch ( MysqlException me ) {
        continue;
      }
      delete object;
      objects().erase( objects().begin() );
    }
    // Get rid of mobs...
    while ( mobs().size() ) {
      if ( !mobs().begin()->second->destroy() ) {
        World::worldLog( ERROR, LOG_WORLD, "Mob %lu failed to destroy itself with area %lu (%s).", mobs().begin()->second->vnum(), ID(), name().c_str() );
        return false;
      }
    }
    // Get rid of this (from memory)...
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete room %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  }

  // Success!
  return true;
}

unsigned long Area::lowestAvailableRoom( void ) {
  unsigned long vnum = 0;

  for ( unsigned long u = low(); u <= high(); ++u ) {
    if ( World::Instance().findRoom( u ) == NULL ) {
      vnum = u;
      break;
    }
  }

  return vnum;
}

void Area::reset( void ) {
  for ( std::map<unsigned long,Room*>::iterator it = rooms().begin(); it != rooms().end(); ++it ) {
    it->second->reset();
  }
  World::Instance().bigBrother( NULL, ADMIN_BIGBRO_RESETS, "Area Reset: %s (%lu)\n", name().c_str(), ID() );
  return;
}

unsigned Area::howManyMobs( const unsigned long& vnum ) {
  unsigned number_of_mobs = 0;
  for ( std::map<unsigned long,Room*>::iterator r_it = rooms().begin(); r_it != rooms().end(); ++r_it ) {
    for ( std::list<Creature*>::iterator c_it = r_it->second->creatures().begin(); c_it != r_it->second->creatures().end(); ++c_it ) {
      if ( *c_it && (*c_it)->isMob() ) {
        if ( ((Mob*)(*c_it))->vnum() == vnum ) {
          number_of_mobs++;
        }
      }
    }
  }
  return number_of_mobs;
}
