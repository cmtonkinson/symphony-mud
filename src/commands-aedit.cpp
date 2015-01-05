/*
 * $Id: commands-aedit.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "avatar.h"
#include "commands-aedit.h"
#include "commandTable.h"
#include "commandTable-default.h"
#include "display.h"
#include "flagTable.h"
#include "io-handler.h"
#include "room.h"
#include "terrainTable.h"
#include "world.h"

/*
ACmd::ACmd( void ) {
  name( "" );
  level( BUILDER );
  addSyntax( 0, "" );
  brief( "" );
  return;
}

bool ACmd::execute( Creature* creature, const std::vector<std::string>& args ) {

  return true;
}
*/

ACmdDelete::ACmdDelete( void ) {
  name( "delete" );
  level( GOD );
  addSyntax( 1, "delete" );
  brief( "Deletes the area.  This is NOT reversible." );
  return;
}

bool ACmdDelete::execute( Creature* creature, const std::vector<std::string>& args ) {
  Area* area = avatar()->aedit();
  CmdExit exit;
  std::vector<std::string> exit_args( 1 );

  if ( args[0] != "delete" ) {
    avatar()->send( printSyntax() );
    return false;
  }

  exit.avatar( avatar() );
  exit.execute( creature, exit_args );
  avatar()->send( "\nYou've deleted Area %lu.\n", area->ID() );
  World::Instance().bigBrother( creature, ADMIN_BIGBRO_CHANGES, "%s has deleted area %lu (%s).", avatar()->identifiers().shortname().c_str(), area->ID(), area->name().c_str() );
  area->destroy( World::Instance().getMysql() );

  return true;
}

ACmdInformation::ACmdInformation( void ) {
  name( "information" );
  level( BUILDER );
  addSyntax( 0, "" );
  brief( "Displays the status of the area." );
  return;
}

bool ACmdInformation::execute( Creature* creature, const std::vector<std::string>& args ) {
  Area* area = avatar()->aedit();
  std::string output;
  char buffer[MAX_BUFFER];
  char names[MAX_BUFFER];
  std::map<unsigned long,std::string> perms;

  for ( std::set<std::pair<unsigned long,unsigned long> >::iterator it = World::Instance().permissions().begin(); it != World::Instance().permissions().end(); ++it ) {
    if ( it->first == area->ID() ) {
      perms[it->second];
    }
  }

  strcpy( names, "{y" );
  if ( perms.empty() ) {
    strcat( names, "none" );
  } else {
    for ( std::map<unsigned long,std::string>::const_iterator it = perms.begin(); it != perms.end(); ++it ) {
      strcat( names, "{y" );
      strcat( names, World::Instance().getAvatarNameByID( it->first ).c_str() );
      strcat( names, "{x, " );
    }
  }
  strcat( names, "{x" );

  sprintf( buffer, "AreaID: {y%lu{x\nFirst vnum: {y%lu{x\nLast vnum: {y%lu{x\nDefault terrain: {y%s{x\nPermission: %s", area->ID(), area->low(), area->high(), area->terrain()->name().c_str(), names );
  output.append( buffer );

  avatar()->send( output );
  return true;
}

ACmdName::ACmdName( void ) {
  name( "name" );
  level( BUILDER );
  addSyntax( -1, "<string>" );
  brief( "Changes the name of the area." );
  return;
}

bool ACmdName::execute( Creature* creature, const std::vector<std::string>& args ) {
  Avatar* avatar = (Avatar*)creature;
  Area* area = avatar->aedit();

  area->name( args[0] );
  avatar->send( "You've reset area %d's name to \"{W%s{x\"!", area->ID(), area->name().c_str() );

  return true;
}

ACmdPermission::ACmdPermission( void ) {
  name( "permission" );
  level( GOD );
  addSyntax( 2, "grant <player>" );
  addSyntax( 2, "revoke <player>" );
  brief( "Updates access privileges for the area." );
  return;
}

bool ACmdPermission::execute( Creature* creature, const std::vector<std::string>& args ) {
  Avatar* target = NULL;

  if ( !(avatar()->adminFlags().test( ADMIN_HEADBUILDER ) || avatar()->level() >= CREATOR ) ) {
    avatar()->send( "Sorry, only the Head Builder can fiddle with permissions." );
    return false;
  }

  if ( args[0] == "grant" ) {
    if ( ( target = World::Instance().findAvatar( args[1] ) ) == NULL ) {
      avatar()->send( "They couldn't be found." );
      return false;
    }
    if ( target->level() < BUILDER ) {
      avatar()->send( "%s isn't a builder.", target->identifiers().shortname().c_str() );
      return false;
    }
    if ( World::Instance().hasPermission( avatar()->aedit(), target ) ) {
      avatar()->send( "%s already has permission to %s.", target->identifiers().shortname().c_str(), avatar()->aedit()->name().c_str() );
      return false;
    }
    World::Instance().givePermission( avatar()->aedit(), target );
    avatar()->send( "You've {Ggranted{x %s access to %s.", target->identifiers().shortname().c_str(), avatar()->aedit()->name().c_str() );
    target->send( "%s has {Ggranted{x you access to %s.", target->seeName( creature, true ).c_str(), avatar()->aedit()->name().c_str() );
  } else if ( args[0] == "revoke" ) {
    if ( ( target = World::Instance().findAvatar( args[1] ) ) == NULL ) {
      avatar()->send( "They couldn't be found." );
      return false;
    }
    if ( !World::Instance().hasPermission( avatar()->aedit(), target ) ) {
      avatar()->send( "%s doesn't have permission to %s.", target->identifiers().shortname().c_str(), avatar()->aedit()->name().c_str() );
      return false;
    }
    World::Instance().removePermission( avatar()->aedit(), target );
    avatar()->send( "You've {rrevoked{x %s's access to %s.", target->identifiers().shortname().c_str(), avatar()->aedit()->name().c_str() );
    target->send( "%s has {rrevoked{x your access to %s.", target->seeName( creature, true ).c_str(), avatar()->aedit()->name().c_str() );
  } else {
    avatar()->send( printSyntax() );
    return false;
  }

  return true;
}

ACmdTerrain::ACmdTerrain( void ) {
  name( "terrain" );
  level( BUILDER );
  addSyntax( 1, "<string>" );
  brief( "Changes the default terrain type of the area." );
  addOptions( "terrain", std::string( "\n" ).append( TerrainTable::Instance().list() ) );
  return;
}

bool ACmdTerrain::execute( Creature* creature, const std::vector<std::string>& args ) {
  Terrain* t = TerrainTable::Instance().find( args[0] );
  if ( t ) {
    avatar()->aedit()->terrain( t );
    avatar()->send( "Area default terrain set to {%c%s{x.", avatar()->aedit()->terrain()->title(), avatar()->aedit()->terrain()->name().c_str() );
    return true;
  } else {
    avatar()->send( "Sorry, '%s{x' isn't a terrain type.", args[0].c_str() );
    return false;
  }
}
