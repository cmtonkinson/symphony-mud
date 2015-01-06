/*
 * $Id: avatar.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include <cstdarg>
#include <time.h>
#include "avatar.h"
#include "commandTable-default.h"
#include "creature.h"
#include "io-handler.h"
#include "estring.h"
#include "object-container.h"
#include "world.h"

Avatar::Avatar( Socket* socket ) {
  time( &_loggedOn );
  status().set( CONNECTING );
  disconnected( false );
  deleteMe( false );
  this->socket( socket );
  identifiers().shortname( estring(this->socket()->getFd()) );
  board( 0 );
  note( NULL );
  gechoColor( 'x' );
  room( NULL );
  mode().set( MODE_NONE );
  title( ", adventurer." );
  level( 1 );
  exp( 0 );
  tnl( 1000 );
  health( 100 );
  maxHealth( 100 );
  mana( 100 );
  maxMana( 100 );
  movement( 100 );
  maxMovement( 100 );
  strength( 14 );
  maxStrength( 18 );
  dexterity( 14 );
  maxDexterity( 18 );
  constitution( 14 );
  maxConstitution( 18 );
  intelligence( 14 );
  maxIntelligence( 18 );
  focus(14);
  maxFocus(18);
  creativity(14);
  maxCreativity(18);
  charisma( 14 );
  maxCharisma( 18 );
  luck(14);
  maxLuck(18);
  armor( 0 );
  bash( 0 );
  slash( 0 );
  pierce( 0 );
  exotic( 0 );
  practices( 5 );
  trains( 5 );
  gains( 5 );
  age( 17 );
  points( 1 );
  gold( 0 );
  silver( 100 );
  bankSilver( 0 );
  bankGold( 0 );
  return;
}

Avatar::~Avatar( void ) {
  delete socket();
  return;
}

/******************************************************* Avatar-specific methods ********************************************************/
/*
 * This methods exists basically to consolidate the underlying actions of
 * the various overloads of Avatar::send() so that we can do things like
 * color in a central location.
 */
void Avatar::processOutput( const std::string& src ) {
  output( output() + src );
  return;
}

bool Avatar::hasInput( void ) {
  try {
    return socket()->hasInput();
  } catch ( SocketException se ) {
    World::Instance().bigBrother( this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::hasInput() -> %s", identifiers().shortname().c_str(), se.getError().c_str() );
    disconnected( true );
    return false;
  }
}

std::string Avatar::getInput( void ) {
  try {
    return socket()->getInput();
  } catch ( SocketException se ) {
    World::Instance().bigBrother( this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::getInput() -> %s", identifiers().shortname().c_str(), se.getError().c_str() );
    disconnected( true );
    return std::string();
  }
}

bool Avatar::hasOutput( void ) {
  try {
    return ( output().length() > 0 );
  } catch ( SocketException se ) {
    World::Instance().bigBrother( this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::hasOutput() -> %s", identifiers().shortname().c_str(), se.getError().c_str() );
    disconnected( true );
    return false;
  }
}

void Avatar::flushOutput( void ) {
  try {
    send( IOhandlers().back()->prompt() );
    socket()->send( output().interpretColor() );
    output( std::string() );
  } catch ( SocketException se ) {
    World::Instance().bigBrother( this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::flushOutput() -> %s", identifiers().shortname().c_str(), se.getError().c_str() );
    disconnected( true );
  }
  return;
}

std::string Avatar::listWhoFlags( void ) {
  return whoFlags().list( FTAvatarWho::Instance(), true );
}

void Avatar::restoreRoom( void ) {
  CmdLook look;
  std::vector<std::string> look_args( 1 );

  // put the Avatar in the right place...
  room( World::Instance().findRoom( roomNumber() ) );
  if ( !room() ) { // just in case
    room( World::Instance().findRoom( 0 ) );
  }
  room()->add( this );
  // let everyone know...
  room()->send_cond( "$p has entered the realm.\n", this );
  // show them around...
  look.avatar( this );
  look.execute( this, look_args );
  return;
}

bool Avatar::create( void ) {
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " INSERT INTO avatars       \
        ( shortname, password )   \
        VALUES                    \
        ( '%s', PASSWORD('%s') );",
      Mysql::addslashes(identifiers().shortname()).c_str(),
      Mysql::addslashes(password()).c_str()
    );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );
    password( " " ); // we don't keep this stored in memory past the creation sequence
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed create player %s: %s\n", identifiers().shortname().c_str(), me.getMessage().c_str() );
    return false;
  }
  return ID();
}

bool Avatar::save( void ) {
  unsigned order = 0;
  bool success = true;

  if ( !isConnected() ) {
    return false;
  }

  /* when saving, we want to undo all equipment attribute modifications before
   * storing Avatar state */
  for ( std::map<int,Object*>::const_iterator it = equipment().objectMap().begin(); it != equipment().objectMap().end(); ++it ) {
    unsetModifications( it->second );
  }

  try {
    Mysql* mysql = World::Instance().getMysql();
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE                          \
          FROM `object_instances`       \
          WHERE `placement` = 'AVATAR'  \
            AND `id` = %lu              \
        ;",
      ID()
    );
    mysql->remove( query );

    for ( std::list<Object*>::const_iterator it = inventory().objectList().begin(); it != inventory().objectList().end(); ++it ) {
      (*it)->saveInstance( mysql, "AVATAR", ID(), 0, order++ );
    }

    order = 0;
    for ( std::map<int,Object*>::const_iterator it = equipment().objectMap().begin(); it != equipment().objectMap().end(); ++it ) {
      it->second->saveInstance( mysql, "AVATAR", ID(), it->first, order++ );
    }

    sprintf( query,
      "UPDATE avatars SET         \
        `shortname` = '%s',       \
        `description` = '%s',     \
        `adminFlags` = %lu,       \
        `gechoColor` = '%c',      \
        `channelFlags` = %lu,     \
        `whoFlags` = %lu,         \
        `room` = %lu,             \
        `title` = '%s',           \
        `poofin` = '%s',          \
        `poofout` = '%s',         \
        `gender` = %u,            \
        `race` = %u,              \
        `cClass` = %u,            \
        `level` = %hd,            \
        `exp` = %lu,              \
        `tnl` = %lu,              \
        `health` = %hu,           \
        `maxHealth` = %hu,        \
        `mana` = %hu,             \
        `maxMana` = %hu,          \
        `movement` = %hu,         \
        `maxMovement` = %hu,      \
        `strength` = %hu,         \
        `maxStrength` = %hu,      \
        `dexterity` = %hu,        \
        `maxDexterity` = %hu,     \
        `constitution` = %hu,     \
        `maxConstitution` = %hu,  \
        `intelligence` = %hu,     \
        `maxIntelligence` = %hu,  \
        `focus` = %hu,            \
        `maxFocus` = %hu,         \
        `creativity` = %hu,       \
        `maxCreativity` = %hu,    \
        `charisma` = %hu,         \
        `maxCharisma` = %hu,      \
        `luck` = %hu,             \
        `maxLuck` = %hu,          \
        `armor` = %hd,            \
        `bash` = %hd,             \
        `slash` = %hd,            \
        `pierce` = %hd,           \
        `exotic` = %hd,           \
        `practices` = %hd,        \
        `trains` = %hd,           \
        `gains` = %hd,            \
        `age` = %hd,              \
        `points` = %hd,           \
        `gold` = %u,              \
        `silver` = %u,            \
        `bank_gold` = %lu,        \
        `bank_silver` = %lu       \
       WHERE avatarID = %lu       \
       LIMIT 1;",
      Mysql::addslashes(identifiers().shortname()).c_str(),
      Mysql::addslashes(identifiers().description()).c_str(),
      adminFlags().value(),
      gechoColor(),
      channelFlags().value(),
      whoFlags().value(),
      room()->vnum(),
      Mysql::addslashes(title()).c_str(),
      Mysql::addslashes(poofin()).c_str(),
      Mysql::addslashes(poofout()).c_str(),
      gender().number(),
      race().number(),
      pClass().number(),
      level(),
      exp(),
      tnl(),
      health(),
      maxHealth(),
      mana(),
      maxMana(),
      movement(),
      maxMovement(),
      strength(),
      maxStrength(),
      dexterity(),
      maxDexterity(),
      constitution(),
      maxConstitution(),
      intelligence(),
      maxIntelligence(),
      focus(),
      maxFocus(),
      creativity(),
      maxCreativity(),
      charisma(),
      maxCharisma(),
      luck(),
      maxLuck(),
      armor(),
      bash(),
      slash(),
      pierce(),
      exotic(),
      practices(),
      trains(),
      gains(),
      age(),
      points(),
      gold(),
      silver(),
      bankGold(),
      bankSilver(),
      ID()
    );
    mysql->update( query );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save avatar %lu: %s\n", ID(), me.getMessage().c_str() );
    success = false;
  }

  /* set all equipment modifications back to where they were */
  for ( std::map<int,Object*>::const_iterator it = equipment().objectMap().begin(); it != equipment().objectMap().end(); ++it ) {
    setModifications( it->second );
  }

  return success;
}

bool Avatar::destroy( void ) {
  unsigned long tempID = ID();
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE FROM           \
        avatars               \
        WHERE avatarID = %lu  \
        LIMIT 1;",
      ID()
    );
    World::Instance().getMysql()->remove( query );
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete mob %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  }
  return true;
}

void Avatar::changeName( const std::string& name ) {
  World::Instance().getAvatars().erase( identifiers().shortname() );
  identifiers().shortname( name );
  World::Instance().getAvatars().insert( std::make_pair( identifiers().shortname(), this ) );
  return;
}

bool Avatar::markForDeletion( const unsigned short& value ) {
  try {
    char query[MAX_BUFFER];
    sprintf( query,
      " UPDATE avatars        \
        SET `delete` = %hu    \
        WHERE avatarID = %lu  \
        LIMIT 1;",
      value ? 1 : 0,
      ID()
    );
    World::Instance().getMysql()->update( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to change deletion marker for player %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }
  return true;
}

bool Avatar::load( void ) {
  Mysql* mysql = World::Instance().getMysql();
  char query[MAX_BUFFER];
  ROW row;
  Object* object = NULL;

  sprintf( query, "SELECT * FROM avatars WHERE LOWER(shortname) = LOWER('%s') AND active = 1 LIMIT 1;", Mysql::addslashes(identifiers().shortname()).c_str() );
  if ( mysql->select( query ) == 1 ) {
    row = mysql->fetch();
    if ( !row.empty() ) {
      ID( row["avatarID"] );
      deleteMe( row["delete"] );
      identifiers().shortname( row["shortname"] );
      identifiers().description( row["description"] );
      identifiers().addKeyword( Regex::lower(identifiers().shortname()) );
      adminFlags().value( row["adminFlags"] );
      gechoColor( row["gechoColor"] );
      channelFlags().value( row["channelFlags"] );
      whoFlags().value( row["whoFlags"] );
      roomNumber( row["room"] );
      title( row["title"] );
      poofin( row["poofin"] );
      poofout( row["poofout"] );
      // identity...
      gender().set( (unsigned)row["gender"] );
      race().set( (unsigned)row["race"] );
      pClass().set( (unsigned) row["cClass"] );
      // stats...
      level( row["level"] );
      exp( row["exp"] );
      tnl( row["tnl"] );
      health( row["health"] );
      maxHealth( row["maxHealth"] );
      mana( row["mana"] );
      maxMana( row["maxMana"] );
      movement( row["movement"] );
      maxMovement( row["maxMovement"] );
      strength( row["strength"] );
      maxStrength( row["maxStrength"] );
      dexterity( row["dexterity"] );
      maxDexterity( row["maxDexterity"] );
      constitution( row["constitution"] );
      maxConstitution( row["maxConstitution"] );
      intelligence( row["intelligence"] );
      maxIntelligence( row["maxIntelligence"] );
      focus( row["focus"] );
      maxFocus( row["maxFocus"] );
      creativity( row["creativity"] );
      maxCreativity( row["maxCreativity"] );
      charisma( row["charisma"] );
      maxCharisma( row["maxCharisma"] );
      luck( row["luck"] );
      maxLuck( row["maxLuck"] );
      armor( row["armor"] );
      bash( row["bash"] );
      slash( row["slash"] );
      pierce( row["pierce"] );
      exotic( row["exotic"] );
      practices( row["practices"] );
      trains( row["trains"] );
      gains( row["gains"] );
      age( row["age"] );
      points( row["points"] );
      gold( row["gold"] );
      silver( row["silver"] );
      bankGold( row["bank_gold"] );
      bankSilver( row["bank_silver"] );
      // post-processing
      whoFlags().clear( WHO_AFK );
      whoFlags().clear( WHO_BUSY );
      // get us some inventory...
      sprintf( query,
        "SELECT *                       \
          FROM `object_instances`       \
          WHERE `placement` = 'AVATAR'  \
            AND `id` = %lu              \
            AND `location` = 0          \
          ORDER BY `order` ASC          \
        ;",
        ID()
      );
      if ( mysql->select( query ) ) {
        while ( row = mysql->fetch() ) {
          object = new Object( row );
          inventory().add( object );
          if ( object->isContainer() ) {
            loadObjectContents( (ObjContainer*)object, row["hash"].c_str() );
          }
        }
      }
      // reclaim equipment from the depths of MySQL...
      sprintf( query,
        "SELECT *                       \
          FROM `object_instances`       \
          WHERE `placement` = 'AVATAR'  \
            AND `id` = %lu              \
            AND `location` != 0         \
          ORDER BY `order` ASC          \
        ;",
        ID()
      );
      if ( mysql->select( query ) ) {
        while ( row = mysql->fetch() ) {
          object = new Object( row );
          equipment().add( object, row["location"] );
          setModifications( object );
          if ( object->isContainer() ) {
            loadObjectContents( object->container(), row["hash"].c_str() );
          }
        }
      }
      return true;
    }
  }

  return false;
}

void Avatar::loadObjectContents( ObjContainer* container, const char* hash ) {
  Object* object = NULL;

  try {
    Mysql conn;
    ROW row;
    char query[MAX_BUFFER];

    sprintf( query,
      "SELECT *                         \
        FROM `object_instances`         \
        WHERE `placement` = 'CONTAINER' \
          AND `in` = '%s'               \
        ORDER BY `order` ASC            \
      ;",
      hash
    );

    if ( conn.select( query ) ) {
      while ( row = conn.fetch() ) {
        object = new Object( row );
        container->inventory().add( object );
        if ( object->isContainer() ) {
          loadObjectContents( object->container(), row["hash"].c_str() );
        }
      }
    }

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to get object contents: %s\n", me.getMessage().c_str() );
    return;
  }

  return;
}

std::string Avatar::stringLoggedOn( void ) {
  return World::realtime( secondsLoggedOn() );
}

/******************************************************* Overloads of virtual methods ********************************************************/
void Avatar::send( const std::string& message ) {
  processOutput( message );
  return;
}

void Avatar::send( const char* format, ... ) {
  char buffer[MAX_BUFFER];
  va_list args;
  va_start( args, format );
  vsprintf( buffer, format, args );
  va_end( args );
  processOutput( buffer );
  return;
}

void Avatar::room( Room* room ) {
  CmdExit exit;
  std::vector<std::string> exit_args( 1 );
  exit.avatar( this );

  Creature::room( room );

  if ( mode().number() == MODE_REDIT ) {
    if ( !World::Instance().hasPermission( this->room()->area(), this ) ) {
      while ( mode().number() != MODE_NONE ) {
        exit.execute( this, exit_args );
      }
      return;
    }
    // Make sure no one else is editing the room...
    for ( std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it ) {
      if ( it->second->isConnected() && it->second != this && it->second->mode().number() == MODE_REDIT && it->second->room() == room ) {
        send( "Sorry, %s is already editing this room.", seeName( it->second ).c_str() );
        while ( mode().number() != MODE_NONE ) {
          exit.execute( this, exit_args );
        }
        return;
      }
    }
  }

  return;
}

void Avatar::title( const std::string& title ) {
  std::string foo = Regex::trim( title );

  _title.clear();

  if ( foo.empty() ) {
    return;
  }

  switch ( foo[0] ) {
    case ',':
    case '.':
    case ':':
    case ';':
      break;
    default:
      _title.assign( 1, ' ' );
  }

  _title.append( foo );

  return;
}

bool Avatar::checkPassword( const std::string& attempt ) {
  Mysql* mysql = World::Instance().getMysql();
  char query[MAX_BUFFER];
  ROW row;

  memset( query, 0, MAX_BUFFER );
  sprintf( query, "SELECT shortname FROM avatars WHERE LOWER(shortname) = LOWER('%s') AND password = PASSWORD('%s') LIMIT 1;",
    Mysql::addslashes(identifiers().shortname()).c_str(),
    Mysql::addslashes(attempt).c_str()
  );
  if ( mysql->select( query ) ) {
    if ( row = mysql->fetch() ) {
      if ( row["shortname"] == identifiers().shortname() ) {
        return true;
      }
    }
  }

  return false;
}
