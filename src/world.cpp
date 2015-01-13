/*
 * $Id: world.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "commandTable.h"
#include "commandTable-default.h"
#include "world.h"
#include "ability-skills.h"

World::World( void ) {
  // basic setup
  booted( time( NULL ) );
  exists( true );
  copyover( false );
  jobsPerTurn( 5 );
  schedule()->add( new RecurringJob( this, &World::tick, 50, 70 ) );
  schedule()->add( new RecurringJob( this, &World::save, 900 ) );
  return;
}

World::~World( void ) {
  return;
}

/************************************************************ CONTROL ************************************************************/
void World::startup( void ) {

  if ( !load() ) {
    worldLog( FATAL, LOG_WORLD, "There was a problem loading a world component." );
  }

  npcIOHandler()->commandTable( &(Commands::Instance()) );
  exist();

  return;
}

void World::exist( const unsigned int& fd ) {
  try {
    if ( copyover() ) {
      getServer()->setFd( fd );
    } else {
      getServer()->startup( 6501 );
    }
  } catch ( SocketException se ) {
    worldLog( FATAL, LOG_SYSTEM, "Couldn't start Socket server: %s", se.getError().c_str() );
    exit( EXIT_FAILED_BOOT );
  }

  if ( copyover() ) {
    worldLog( SYSTEM, LOG_SYSTEM, "System rebooted successfully." );
  } else {
    worldLog( SYSTEM, LOG_SYSTEM, "System up on port %d.", getServer()->getPort() );
  }

  while ( exists() ) {
    turn();
  }

  if ( !save() ) {
    worldLog( FATAL, LOG_WORLD, "There was a problem saving a world component." );
  }

  worldLog( SYSTEM, LOG_SYSTEM, "System shutting down." );

  return;
}

void World::turn( void ) {
  handleNewConnections();
  handleJobs();
  handleInput();
  handleOutput();
  handleDisconnects();
  usleep( 1000 ); // to throttle the CPU usage
  return;
}

bool World::reboot( Creature* creature ) {
  std::map<std::string,Avatar*>::iterator it;
  estring fd;
  FILE* fp = NULL;

  fp = fopen( REBOOT_FILE, "w" );

  if ( !fp ) {
    if ( creature ) {
      creature->send( "Copyover file couldn't be opened for writing! Copyover aborted!" );
    }
    worldLog( ERROR, LOG_SYSTEM, "Copyover file couldn't be opened for writing! Copyover aborted!" );
    return false;
  }

  for ( it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
    if ( it->second->isConnected() ) {
      fprintf( fp, "%d %s %s\n", it->second->socket()->getFd(), it->second->socket()->getIP().c_str(), it->second->identifiers().shortname().c_str() );
    }
  }

  fclose( fp );

  broadcast( "Hold on to your pants; Copyover in progress..." );
  handleOutput();

  if ( save() ) {
    worldLog( SYSTEM, LOG_SYSTEM, "World saved pre-reboot." );
  } else {
    worldLog( FATAL, LOG_WORLD, "There was a problem saving a world component." );
    broadcast( "Copyover aborted." );
    return false;
  }

  worldLog( SYSTEM, LOG_SYSTEM, "System going down for reboot." );

  fd = estring( "-fd=" ).append( estring( getServer()->getFd() ) );
  if ( execl( "./symphony", "-copyover", fd.c_str(), NULL ) < 0 ) {
    worldLog( ERROR, LOG_SYSTEM, "execl(): %s", strerror( errno ) );
    broadcast( "Copyover failed." );
  }

  return false;
}

void World::recover( const unsigned int& fd ) {
  FILE* fp = NULL;
  int client_fd = 0;
  char ip[MAX_BUFFER];
  char name[MAX_BUFFER];
  Avatar* avatar = NULL;

  if ( !load() ) {
    worldLog( FATAL, LOG_WORLD, "There was a problem loading a world component." );
  }

  fp = fopen( REBOOT_FILE, "r" );

  if ( !fp ) {
    worldLog( FATAL, LOG_SYSTEM, "Copyover file couldn't be opened for reading. Copyover failed." );
    exit( EXIT_FAILED_REBOOT );
  }

  while ( 1 ) {
    if ( fscanf( fp, "%d %s %s", &client_fd, ip, name ) < 3 ) {
      break;
    }
    avatar = new Avatar( new Socket( client_fd ) );
    avatar->socket()->setIP( ip );
    avatar->identifiers().shortname( name );
    avatar->load();
    avatar->pushIOHandler( new InputIOHandler( avatar ) );
    insert( avatar );
    avatar->status().set( CONNECTED );
    avatar->restoreRoom();
    avatar->send( "\n\n" );
  }

  fclose( fp );
  unlink( REBOOT_FILE );

  broadcast( "\n\nCopyover successful. You may now drop your pants." );

  exist( fd );

  return;
}

bool World::load( void ) {
  bool status = true;

  if ( !loadAreas( getMysql() ) ) {
    status = false;
  }

  loadPermissions();

  if ( !loadSocials() ) {
    status = false;
  }

  add( new Board( BOARD_GENERAL ) );
  add( new Board( BOARD_CHANGES ) );
  add( new Board( BOARD_ADMIN ) );
  add( new Board( BOARD_NEWS ) );
  add( new Board( BOARD_IMMORTAL ) );
  add( new Board( BOARD_BUILDING ) );

  return status;
}

bool World::save( void ) {
  bool status = true;

  if ( !saveAreas( getMysql() ) ) {
    status = false;
  }

  for ( std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ++a_it ) {
    if ( a_it->second->isConnected() ) {
      a_it->second->save();
    }
  }

  saveSocials();

  worldLog( SYSTEM, LOG_WORLD, "World::save()" );

  return status;
}

bool World::toggleCommand( char table_prefix, std::string command_name, bool enabled ) {
  try {
    Mysql* mysql = getMysql();
    char query[MAX_BUFFER];
    CommandTable* table = NULL;
    Command* command = NULL;

    switch ( table_prefix ) {
      case 'x': table = &(Commands::Instance());      break;
      case 'A': table = &(AeditCommands::Instance()); break;
      case 'M': table = &(MeditCommands::Instance()); break;
      case 'O': table = &(OeditCommands::Instance()); break;
      case 'P': table = &(PeditCommands::Instance()); break;
      case 'R': table = &(ReditCommands::Instance()); break;
      case 'T': table = &(TeditCommands::Instance()); break;
      default:  table = NULL;                         break;
    }

    if ( table ) {
      if ( ( command = table->find( command_name ) ) != NULL ) {
        if ( command->level() >= CREATOR ) {
          return false;
        }
        command->enabled( enabled );
        if ( enabled ) {
          sprintf( query, "DELETE FROM disabled_commands WHERE `table` = '%c' AND `name` = '%s';", table_prefix, Mysql::addslashes(command->name()).c_str() );
          mysql->remove( query );
        } else {
          sprintf( query, "INSERT IGNORE INTO disabled_commands ( `table`, `name` ) VALUES ( '%c', '%s' );", table_prefix, Mysql::addslashes(command->name()).c_str() );
          mysql->insert( query );
        }
        return true;
      }
    }

    return false;
  } catch ( MysqlException me ) {
    return false;
  }

}

bool World::loadSocials( void ) {
  try {
    Mysql* mysql = getMysql();
    ROW row;
    char query[MAX_BUFFER];
    sprintf( query, "SELECT * FROM socials ORDER BY name ASC;" );
    if ( mysql->select( query ) ) {
      while ( row = mysql->fetch() ) {
        Commands::Instance().add( new SocialCommand( row ) );
      }
    }
    return true;
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load social commands: %s\n", me.getMessage().c_str() );
    return false;
  }

  return false;
}

void World::saveSocials( void ) {
  for ( std::vector<Command*>::iterator it = Commands::Instance().commands().begin(); it != Commands::Instance().commands().end(); ++it ) {
    if ( (*it)->social() ) {
      ((SocialCommand*)*it)->save();
    }
  }
  return;
}

bool World::tick( RecurringJob* job ) {
  bigBrother( NULL, ADMIN_BIGBRO_EVENTS, "Tick!" );
  return true;
}

/************************************************************ I/O ************************************************************/
void World::handleInput( void ) {
  std::map<std::string,Avatar*>::iterator next;
  for ( std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ) {
    next = a_it;
    next++;
    try {
      if ( a_it->second->hasInput() ) {
        a_it->second->handle();
      }
    } catch ( SocketException se ) {
      bigBrother( a_it->second, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in World::handleInput() -> %s", a_it->second->identifiers().shortname().c_str(), se.getError().c_str() );
    }
    a_it = next;
  }
  return;
}

void World::handleOutput( void ) {
  std::map<std::string,Avatar*>::iterator next;
  for ( std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ++a_it ) {
    try {
      if ( a_it->second->hasOutput() ) {
        a_it->second->flushOutput();
      }
    } catch ( SocketException se ) {
      bigBrother( a_it->second, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in World::handleOutput() -> %s", a_it->second->identifiers().shortname().c_str(), se.getError().c_str() );
    }
  }
  return;
}

void World::handleNewConnections( void ) {
  std::map<std::string,Avatar*>::iterator a_it, next, foo;
  std::vector<Socket*> new_connections = getServer()->accept();
  std::vector<Socket*>::iterator sock_it;
  Avatar* avatar = NULL;
  for ( sock_it = new_connections.begin(); !new_connections.empty(); ) {
    avatar = new Avatar( *sock_it );
    insert( avatar );
    avatar->pushIOHandler( new LoginBannerIOHandler( avatar ) );
    bigBrother( NULL, ADMIN_BIGBRO_SYSTEM, "New connection from %s!", avatar->socket()->getIP().c_str() );
    new_connections.erase( sock_it );
  }
  return;
}

void World::handleDisconnects( void ) {
  std::map<std::string,Avatar*>::iterator next;
  for ( std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ) {
    if ( a_it->second->disconnected() ) {
      /* We have to go through a bit of a song and dance here, because map::erase() does
       * not return the next valid iterator in the sequence like vector::erase() does.
       */
      next = a_it;
      ++next;
      bigBrother( a_it->second, ADMIN_BIGBRO_LOGINS, "%s has logged out from %s.", a_it->second->identifiers().shortname().c_str(), a_it->second->socket()->getIP().c_str() );
      playerLog( NOTICE, LOG_PLAYER, "%s (%lu) logged in from %s", a_it->second->identifiers().shortname().c_str(), a_it->second->ID(), a_it->second->socket()->getIP().c_str() );
      if ( a_it->second->room() ) {
        a_it->second->room()->send_cond( "$p has left the realm.", a_it->second );
        a_it->second->room()->remove( a_it->second );
      }
      remove( a_it->second );
      if ( a_it->second->deleteMe() ) { // permanently delete this account
        a_it->second->destroy();
      } else { // just logging out
        delete a_it->second;
      }
      a_it = next;
    } else {
      ++a_it;
    }
  }
  return;
}

void World::broadcast( const std::string& message ) {
  for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); it++ ) {
    if ( it->second->isConnected() ) {
      it->second->send( message );
      it->second->send( "\n" );
    }
  }
  return;
}

void World::bigBrother( Creature* creature, const unsigned long& type, const char* format, ... ) {
  char buffer[MAX_BUFFER];
  va_list args;

  va_start( args, format );
  vsprintf( buffer, format, args );
  va_end( args );

  if ( creature && type ) {
    for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
      if ( it->second->isConnected() && it->second->adminFlags().test( ADMIN_BIGBROTHER ) && it->second->adminFlags().test( type ) && it->second->level() >= creature->level() && it->second != creature ) {
        it->second->send( "BigBrother: %s\n", buffer );
      }
    }
  } else if ( type ) {
    for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
      if ( it->second->isConnected() && it->second->adminFlags().test( ADMIN_BIGBROTHER ) && it->second->adminFlags().test( type ) ) {
        it->second->send( "BigBrother: %s\n", buffer );
      }
    }
  } else {
    for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
      if ( it->second->isConnected() && it->second->adminFlags().test( ADMIN_BIGBROTHER ) ) {
        it->second->send( "BigBrother: %s\n", buffer );
      }
    }
  }

  return;
}

/************************************************************ JOBS ************************************************************/
void World::handleJobs( void ) {
  unsigned count = 0;
  while ( schedule()->fire() ) {
    if ( ++count >= jobsPerTurn() ) {
      break;
    }
  }
  return;
}

/************************************************************ CREATURES ************************************************************/
void World::insert( Creature* creature ) {
  getCreatures().insert( creature );
  if ( creature->isAvatar() ) {
    getAvatars().insert( std::make_pair( creature->identifiers().shortname(), (Avatar*)creature ) );
  }
  return;
}

void World::remove( Creature* creature ) {
  getCreatures().erase( creature );
  if ( creature->isAvatar() ) {
    getAvatars().erase( creature->identifiers().shortname() );
  }
  return;
}

Creature* World::findCreature( const std::string& name ) {
  for ( std::set<Creature*>::iterator it = getCreatures().begin(); it != getCreatures().end(); ++it ) {
    if ( (*it)->identifiers().matchesKeyword( name ) ) {
      if ( (*it)->isAvatar() ) {
        if ( ((Avatar*)*it)->isConnected() ) {
          return *it;
        }
      } else {
        return *it;
      }
    }
  }
  return NULL;
}

/* Avatar-specific search method (slightly more efficient when searching only for PCs) */
Avatar* World::findAvatar( const std::string& name ) {
  for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
    if ( it->second->isConnected() && it->second->identifiers().matchesKeyword( name ) ) {
      return it->second;
    }
  }
  return NULL;
}

bool World::transport( Creature* creature, const unsigned long& vnum ) {
  Room* room = NULL;
  CmdLook look;
  std::vector<std::string> look_args( 1 );

  if ( ( room = findRoom( vnum ) ) == NULL ) {
    return false;
  }

  creature->room()->remove( creature );
  room->add( creature );
  creature->room( room );
  creature->send( "You've been transported to %s.\n", creature->room()->name().c_str() );
  look.execute( creature, look_args );
  return true;
}

/************************************************************ AVATARS ************************************************************/
Avatar* World::findAvatar( const unsigned long& ID ) {
  for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
    if ( it->second->isConnected() && it->second->ID() == ID ) {
      return it->second;
    }
  }
  return NULL;
}

std::string World::getAvatarNameByID( const unsigned long& ID ) {
  try {
    ROW row;
    char query[MAX_BUFFER];

    sprintf( query,
      " SELECT shortname      \
        FROM avatars          \
        WHERE avatarID = %lu  \
        LIMIT 1;",
      ID
    );
    if ( getMysql()->select( query ) ) {
      if ( row = getMysql()->fetch() ) {
        return row["shortname"];
      }
    }
    return std::string("none");

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save area %lu: %s\n", ID, me.getMessage().c_str() );
    return std::string("error");
  }
}

bool World::removeAvatar( const std::string& name ) {
  for ( std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it ) {
    if ( it->first == name ) {
      getCreatures().erase( it->second );
      getAvatars().erase( name );
      return true;
    }
  }
  return false;
}

/************************************************************ AREAS ************************************************************/
bool World::loadAreas( Mysql* db ) {
  Area* area = NULL;
  ROW row;

  // Grab areas...
  try {
    if ( db->select( "SELECT * FROM areas ORDER BY low ASC;" ) ) {
      while ( row = db->fetch() ) {
        area = new Area();
        area->load( row );
        insert( area );
      }
    }
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load areas: %s\n", me.getMessage().c_str() );
    return false;
  }

  // Load World components...
  for ( std::set<Area*,area_comp>::iterator it = getAreas().begin(); it != getAreas().end(); ++it ) {
    try {
      (*it)->loadMobs();
      (*it)->loadObjects( db );
      (*it)->loadRooms();
      (*it)->loadExits();
      (*it)->reset();
    } catch ( MysqlException me ) {
      fprintf( stderr, "Failed to fully load area %lu: %s\n", (*it)->ID(), me.getMessage().c_str() );
    }
  }

  return true;
}

bool World::saveAreas( Mysql* db ) {
  bool status = true;
  for ( std::set<Area*,area_comp>::iterator it = getAreas().begin(); it != getAreas().end(); ++it ) {
    try {
      (*it)->save( db );
    } catch ( MysqlException me ) {
      status = false;
    }
  }
  return status;
}

void World::insert( Area* area ) {
  getAreas().insert( area );
  return;
}

void World::remove( Area* area ) {
  getAreas().erase( area );
  return;
}

Area* World::findArea( const unsigned long& ID ) {
  for ( std::set<Area*,area_comp>::iterator it = getAreas().begin(); it != getAreas().end(); ++it ) {
    if ( (*it)->ID() == ID ) {
      return *it;
    }
  }
  return NULL;
}

Area* World::lookup( const unsigned long& vnum ) {
  for ( std::set<Area*,area_comp>::iterator it = getAreas().begin(); it != getAreas().end(); ++it ) {
    if ( (*it)->low() <= vnum && vnum <= (*it)->high() ) {
      return *it;
    }
  }
  return NULL;
}

/************************************************************ AREA PERMISSIONS ************************************************************/
bool World::hasPermission( Area* area, Avatar* avatar ) {
  // Only the Administrator can fiddle with Limbo...
  if ( area->ID() == 1 && avatar->level() < CREATOR ) {
    return false;
  }
  // The Administrator and the Head Builder can edit anything else...
  if ( avatar->adminFlags().test( ADMIN_HEADBUILDER ) || avatar->level() >= CREATOR ) {
    return true;
  }
  // The average Joe needs explicit permission for a given area...
  if ( permissions().find( std::make_pair( area->ID(), avatar->ID() ) ) != permissions().end() ) {
    return true;
  }
  return false;
}

void World::givePermission( Area* area, Avatar* avatar ) {
  permissions().insert( std::make_pair( area->ID(), avatar->ID() ) );
  try {
    char query[MAX_BUFFER];
    sprintf( query, "INSERT IGNORE INTO permissions ( areaID, avatarID ) VALUES ( %lu, %lu );", area->ID(), avatar->ID() );
    getMysql()->insert( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create area permission: %s\n", me.getMessage().c_str() );
  }
  return;
}

void World::removePermission( Area* area, Avatar* avatar ) {
  permissions().erase( std::make_pair( area->ID(), avatar->ID() ) );
  try {
    char query[MAX_BUFFER];
    sprintf( query, "DELETE FROM permissions WHERE areaID = %lu AND avatarID = %lu;", area->ID(), avatar->ID() );
    getMysql()->remove( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create area permission: %s\n", me.getMessage().c_str() );
  }
  return;
}

std::vector<Avatar*> World::getPermissions( Area* area ) {
  std::vector<Avatar*> avatars;
  Avatar* avatar = NULL;

  for ( std::set<std::pair<unsigned long,unsigned long> >::iterator it = permissions().begin(); it != permissions().end(); ++it ) {
    if ( it->first == area->ID() ) {
      if ( ( avatar = findAvatar( it->second ) ) != NULL ) {
        avatars.push_back( avatar );
      }
    }
  }

  return avatars;
}

std::vector<Area*> World::getPermissions( Avatar* avatar ) {
  std::vector<Area*> areas;
  Area* area = NULL;

  for ( std::set<std::pair<unsigned long,unsigned long> >::iterator it = permissions().begin(); it != permissions().end(); ++it ) {
    if ( it->second == avatar->ID() ) {
      if ( ( area = findArea( it->first ) ) != NULL ) {
        areas.push_back( area );
      }
    }
  }

  return areas;
}

void World::loadPermissions( void ) {
  try {
    Mysql* mysql = getMysql();
    ROW row;
    // Load Area permissions...
    if ( mysql->select( "SELECT areaID, avatarID FROM permissions;" ) ) {
      while ( row = mysql->fetch() ) {
        permissions().insert( std::make_pair( row["areaID"], row["avatarID"] ) );
      }
    }
    // Load disabled commands...
    if ( mysql->select( "SELECT `table`, `name` FROM disabled_commands;" ) ) {
      while ( row = mysql->fetch() ) {
        toggleCommand( (char)row["table"], row["name"], false );
      }
    }
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to load area permissions: %s\n", me.getMessage().c_str() );
  }
  return;
}

/************************************************************ ROOMS ************************************************************/
Room* World::findRoom( const unsigned long& vnum ) {
  std::map<unsigned long,Room*>::iterator r_it;
  std::map<unsigned long,Room*>::iterator r_end;
  for ( std::set<Area*,area_comp>::iterator a_it = getAreas().begin(); a_it != getAreas().end(); ++a_it ) {
    r_end = (*a_it)->rooms().end();
    if ( ( r_it = (*a_it)->rooms().find( vnum ) ) != r_end ) {
      return r_it->second;
    }
  }
  return NULL;
}

/************************************************************ OBJECTS ************************************************************/
void World::insert( Object* object ) {
  getObjects().insert( object );
  return;
}

void World::remove( Object* object ) {
  getObjects().erase( object );
  return;
}

Object* World::findObject( const std::string& name ) {
  std::set<Object*>::iterator it;
  std::set<Object*>::iterator end = getObjects().end();
  for ( it = getObjects().begin(); it != end; ++it ) {
    if ( (*it)->identifiers().matchesKeyword( name ) ) {
      return *it;
    }
  }
  return NULL;
}

bool World::transport( Object* object, Room* from, const unsigned long& vnum ) {
  Room* to = NULL;

  if ( ( to = findRoom( vnum ) ) == NULL ) {
    return false;
  }

  from->remove( object );
  from->send( "$o disappears in small puff of smoke.\n", NULL, object );
  to->add( object );
  to->send( "$o appears with a small \"pop!\"\n", NULL, object );
  return true;
}

/************************************************************ BOARDS ************************************************************/
void World::add( Board* board ) {
  boards().insert( std::make_pair( board->number(), board ) );
  return;
}

void World::add( const unsigned short& board, Note* note ) {
  boards()[board]->notes().insert( std::make_pair( note->ID(), note ) );
  return;
}

void World::saveBoards( void ) {
  for ( std::map<unsigned,Board*>::iterator it = boards().begin(); it != boards().end(); ++it ) {
    it->second->save();
  }
  return;
}

/************************************************************ STATIC ************************************************************/
unsigned World::rand( const unsigned& min, const unsigned& max ) {
  return ( ::rand() % (max-min+1) + min );
}

void World::worldLog( const unsigned short& level, const unsigned short& type, const char* format, ... ) {
  char buffer[MAX_BUFFER];
  char query[MAX_BUFFER];
  va_list args;

  // Process our arguments...
  va_start( args, format );
  vsprintf( buffer, format, args );
  va_end( args );

  // Prep and send our query...
  sprintf( query, "INSERT INTO world_log ( level, type, text ) VALUES ( %d, %d, '%s' );", level, type, Mysql::addslashes(buffer).c_str() );

  try {
    World::Instance().getMysql()->insert( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to log world event: %s\n", query );
  }

  // Depending on the level (and the current system log level) we may also print the message...
  if ( level == SYSTEM || level >= WARNING ) {
    Instance().bigBrother( NULL, ADMIN_BIGBRO_SYSTEM, buffer );
    fprintf( stderr, "Log %s: %s\n", strnow().c_str(), buffer );
  }

  return;
}

void World::playerLog( const unsigned short& level, const unsigned short& type, const char* format, ... ) {
  char buffer[MAX_BUFFER];
  char query[MAX_BUFFER];
  va_list args;

  // Process our arguments...
  va_start( args, format );
  vsprintf( buffer, format, args );
  va_end( args );

  // Prep and send our query...
  sprintf( query, "INSERT INTO player_log ( level, type, text ) VALUES ( %d, %d, '%s' );", level, type, Mysql::addslashes(buffer).c_str() );

  try {
    World::Instance().getMysql()->insert( query );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to log player event: %s\n", query );
  }

  return;
}

unsigned long World::now( void ) {
  return time( NULL );
}

std::string World::strnow( void ) {
  char buffer[MAX_BUFFER];
  time_t seconds = time( NULL );
  tm* info = localtime( &seconds );
  strftime( buffer, MAX_BUFFER, "%Y/%m/%d %H:%M:%S", info );
  return buffer;
}

std::string World::strtime( time_t in_time ) {
  char buffer[MAX_BUFFER];
  time_t seconds = 0;
  if ( in_time ) {
    seconds = in_time;
  } else {
    seconds = time( NULL );
  }
  tm* info = localtime( &seconds );
  strftime( buffer, MAX_BUFFER, "%A, %B %d, %Y  %H:%M:%S", info );
  return buffer;
}

std::string World::realtime( const unsigned long& seconds, unsigned short granularity ) {
  char buffer[MAX_BUFFER];
  char foo[100];
  register unsigned long sec = seconds;
  register unsigned long min = 0;
  register unsigned long hour = 0;
  register unsigned long day = 0;
  register unsigned long year = 0;

  memset( buffer, 0, MAX_BUFFER );

  while ( sec >= 60 ) {
    ++min;
    sec -= 60;
  }

  if ( granularity > GRAN_MINUTE ) {
    while ( min >= 60 ) {
      ++hour;
      min -= 60;
    }
    if ( granularity > GRAN_HOUR ) {
      while ( hour >= 24 ) {
        ++day;
        hour -= 24;
      }
      if ( granularity > GRAN_DAY ) {
        while ( day >= 365 ) {
          ++year;
          day -= 365;
        }
      }
    }
  }

  if ( year ) {
    if ( year == 1 ) {
      sprintf( foo, "%lu year", year );
    } else {
      sprintf( foo, "%lu years", year );
    }
    strcat( buffer, foo );
  }

  if ( day ) {
    if ( day == 1 ) {
      sprintf( foo, "%lu day", day );
    } else {
      sprintf( foo, "%lu days", day );
    }
    if ( year ) {
      strcat( buffer, ", " );
    }
    strcat( buffer, foo );
  }

  if ( hour ) {
    if ( hour == 1 ) {
      sprintf( foo, "%lu hour", hour );
    } else {
      sprintf( foo, "%lu hours", hour );
    }
    if ( year || day ) {
      strcat( buffer, ", " );
    }
    strcat( buffer, foo );
  }

  if ( min ) {
    if ( min == 1 ) {
      sprintf( foo, "%lu minute", min );
    } else {
      sprintf( foo, "%lu minutes", min );
    }
    if ( year || day || hour ) {
      strcat( buffer, ", " );
    }
    strcat( buffer, foo );
  }

  if ( sec ) {
    if ( sec == 1 ) {
      sprintf( foo, "%lu second", sec );
    } else {
      sprintf( foo, "%lu seconds", sec );
    }
    if ( year || day || hour || min ) {
      strcat( buffer, ", " );
    }
    strcat( buffer, foo );
  }

  return buffer;
}

bool World::search_map( Creature* creature, Room*** map, const unsigned short& ymax, const unsigned short& xmax, const short& y, const short& x, Room* room, std::string** display ) {
  Exit* exit = NULL;
  short new_x = 0;
  short new_y = 0;
  short dis_x = 0;
  short dis_y = 0;
  std::string dis_s;
  // Bounds check...
  if ( y < 0 || y > ymax*2 || x < 0 || x > xmax*2 ) {
    return true;
  }
  // There's a room at this location...
  if ( map[y][x] ) {
    // Collision check...
    if ( map[y][x] != room ) {
      return false;
    }
    // We've already been here; bail out...
    return true;
  }
  // Store our pointer and map position...
  map[y][x] = room;
  display[y*2][x*2] = get_marker( creature, room );
  // Search from here...
  for ( register unsigned short u = NORTH; u < UP; ++u ) {
    new_x = x;
    new_y = y;
    switch (u) {
      case NORTH: new_y++;  dis_x = new_x*2;    dis_y = new_y*2-1;  dis_s = "|"; break;
      case EAST:  new_x++;  dis_x = new_x*2-1;  dis_y = new_y*2;    dis_s = "-"; break;
      case SOUTH: new_y--;  dis_x = new_x*2;    dis_y = new_y*2+1;  dis_s = "|"; break;
      case WEST:  new_x--;  dis_x = new_x*2+1;  dis_y = new_y*2;    dis_s = "-"; break;
      default: return false;
    }
    if ( ( exit = room->exit( u ) ) && new_y >= 0 && new_x >= 0 ) {
      if ( exit->flags().test( EXIT_HIDDEN ) || exit->target()->flags().test( ROOM_NOMAP ) ) {
        continue;
      }
      if ( !World::search_map( creature, map, ymax, xmax, new_y, new_x, room->exit( u )->target(), display ) ) {
        return false;
      }
      display[dis_y][dis_x] = dis_s;
    }
  }
  return true;
}

std::string World::get_marker( Creature* creature, Room* room ) {
  char buf[8];
  if ( creature->room() == room ) {
    return "{c@{x";
  } else {
    sprintf( buf, "{%c%c{x", room->terrain()->map(), '+' );
    return buf;
  }
}
