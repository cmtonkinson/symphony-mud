/*
 * $Id: command.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "command.h"
#include "commandTable.h"
#include "display.h"
#include "room.h"
#include "socket.h"
#include "world.h"

///////////////////////////////////////////// BASE CLASS /////////////////////////////////////////////
Command::Command( void ) {
  enabled( true );
  playerOnly( false );
  special( false );
  social( false );
  level( 1 );
  allowedPositions( ~0 );
  commandTable( NULL );
  avatar( NULL );
  return;
}

Command::~Command( void ) {
  return;
}

void Command::level( const unsigned short& level ) {
  _level = level;
  if ( _level > LEVEL_HERO ) {
    playerOnly( true );
  }
  return;
}

void Command::addSyntax( const short& num_args, const std::string& usage ) {
  arguments().insert( num_args );
  syntax().push_back( usage );
  return;
}

const char* Command::printSyntax( void ) const {
  std::string dest;
  bool first = true;
  dest.assign( "{g" ).append( name() ).append( "{x is a level {g" ).append( estring( level() ) ).append( "{x command.\n\n" );
  dest.append( "{gSynopsis:{x " ).append( brief() ).append( "\n" );
  for ( std::vector<std::string>::const_iterator it = syntax().begin(); it != syntax().end(); ++it ) {
    if ( first ) {
      dest.append( "{gUsage:{x " ).append( name() ).append( 1, ' ' ).append( *it ).append( 1, '\n' );
      first = false;
    } else {
      dest.append( "       " ).append( name() ).append( 1, ' ' ).append( *it ).append( 1, '\n' );
    }
  }
  if ( !options().empty() ) {
    dest.append( 1, '\n' );
  }
  for ( OptionMap::const_iterator it = options().begin(); it != options().end(); ++it ) {
    dest.append( "Options for {g" ).append( it->first ).append( "{x: " ).append( it->second ).append( 1, '\n' );
  }
  return dest.c_str();
}

void Command::addOptions( const std::string& argument, const std::string& option_list ) {
  options().insert( std::make_pair( argument, option_list ) );
  return;
}

///////////////////////////////////////////// SOCIALS /////////////////////////////////////////////
SocialCommand::SocialCommand( void ) {
  return;
}

SocialCommand::SocialCommand( const std::string& name, const unsigned long& avatarID ): Command() {
  this->name( name );
  social( true );
  ID( 0 );
  creator( avatarID );
  targetNone( false );
  targetSelf( false );
  targetVictim( false );
  noneActor( "You do something." );
  noneRoom( "$c does something." );
  selfActor( "You do something to yourself." );
  selfRoom( "$c does something to $f." );
  victimActor( "You do something to $C." );
  victimVictim( "$c does something to you." );
  victimRoom( "$c does something to $C." );

  try {
    char query[MAX_BUFFER];
    sprintf( query, "INSERT IGNORE INTO socials ( name, creator ) VALUES ( '%s', %lu );", Mysql::addslashes(name).c_str(), avatarID );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );
    Commands::Instance().add( this );
  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create social command \"%s\": %s\n", this->name().c_str(), me.getMessage().c_str() );
    return;
  }

  return;
}

SocialCommand::SocialCommand( ROW row ) {
  social( true );
  ID( row["id"] );
  creator( row["creator"] );
  name( row["name"] );
  flags().value( row["flags"] );
  targetNone( row["targetNone"] );
  targetSelf( row["targetSelf"] );
  targetVictim( row["targetVictim"] );
  noneActor( row["noneActor"] );
  noneRoom( row["noneRoom"] );
  selfActor( row["selfActor"] );
  selfRoom( row["selfRoom"] );
  victimActor( row["victimActor"] );
  victimVictim( row["victimVictim"] );
  victimRoom( row["victimRoom"] );
  if ( targetNone() ) {
    arguments().insert( 0 );
  }
  if ( targetSelf() || targetVictim() ) {
    arguments().insert( 1 );
  }
  if ( arguments().empty() ) {
    // In case the author of the social smoked his breakfast and
    // forgot to activate any modes of operation for the command
    // this will keep the engine from seg faulting in IOHandler::handle().
    arguments().insert( 0 );
  }
  return;
}

SocialCommand::~SocialCommand( void ) {
  return;
}

void SocialCommand::save( void ) {
  try {
    char query[MAX_BUFFER];

    sprintf( query, "               \
          UPDATE socials            \
          SET creator = %lu,        \
            flags = %lu,            \
            targetNone = %u,        \
            targetSelf = %u,        \
            targetVictim = %u,      \
            noneActor = '%s',       \
            noneRoom = '%s',        \
            selfActor = '%s',       \
            selfRoom = '%s',        \
            victimActor = '%s',     \
            victimVictim = '%s',    \
            victimRoom = '%s'       \
          WHERE id = %lu            \
          ;",
      creator(),
      flags().value(),
      targetNone(),
      targetSelf(),
      targetVictim(),
      Mysql::addslashes(noneActor()).c_str(),
      Mysql::addslashes(noneRoom()).c_str(),
      Mysql::addslashes(selfActor()).c_str(),
      Mysql::addslashes(selfRoom()).c_str(),
      Mysql::addslashes(victimActor()).c_str(),
      Mysql::addslashes(victimVictim()).c_str(),
      Mysql::addslashes(victimRoom()).c_str(),
      ID()
    );
    World::Instance().getMysql()->update( query );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save command %lu (\"%s\"): %s\n", ID(), name().c_str(), me.getMessage().c_str() );
    return;
  }

  return;
}

bool SocialCommand::destroy( void ) {
  unsigned long tempID = ID();
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE          \
        FROM socials    \
        WHERE id = %lu  \
        LIMIT 1;",
      ID()
    );
    World::Instance().getMysql()->remove( query );
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete social %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  }
  return true;
}

bool SocialCommand::execute( Creature* creature, const std::vector<std::string>& args ) {
  Creature* victim = NULL;

  if ( args[0].empty() ) {
    if ( targetNone() ) {
      creature->send( noneActor() );
      creature->room()->send_cond( noneRoom().c_str(), creature, creature, NULL, TO_ROOM, isAudible() );
      return true;
    }
  } else if ( args.size() == 1 ) {
    victim = creature->findCreature( args[0] );
    if ( victim == NULL ) {
      creature->send( "They're not around at the moment." );
      return false;
    }
    if ( victim == creature ) {
      if ( targetSelf() ) {
        creature->send( selfActor() );
        creature->room()->send_cond( selfRoom().c_str(), creature, creature, creature, TO_ROOM, isAudible() );
        return true;
      } else {
        creature->send( "You'd look pretty silly doing that to yourself." );
        return false;
      }
    } else {
      if ( targetVictim() ) {
        creature->send( Display::formatAction( victimActor().c_str(), creature, creature, victim, creature ) );
        // Unless the social is audible or tactile, the victim shouldn't see it if they can't see the actor...
        if ( victim->canSee( creature ) > SEE_NOTHING || isAudible() || isTactile() ) {
          victim->send( Display::formatAction( victimVictim().c_str(), creature, creature, victim, victim ) );
        }
        creature->room()->send_cond( victimRoom().c_str(), creature, creature, victim, TO_NOTVICT, isAudible() );
        return true;
      } else {
        creature->send( "You can't do that to someone else." );
        return false;
      }
    }
  }

  creature->send( "social error" );
  return false;
}
