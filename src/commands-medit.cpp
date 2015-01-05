/*
 * $Id: commands-medit.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "commands-medit.h"
#include "creature.h"
#include "display.h"
#include "io-handler.h"
#include "mob.h"
#include "room.h"

/*MCmd::MCmd( void ) {
  name( "" );
  level( BUILDER );
  addSyntax( 0, "" );
  brief( "" );
  return;
}

bool MCmd::execute( Creature* creature, const std::vector<std::string>& args ) {

  return true;
}
*/

MCmdDescription::MCmdDescription( void ) {
  name( "description" );
  level( BUILDER );
  addSyntax( 0, "" );
  brief( "Invokes the Text Editor for the Mobs' description." );
  return;
}

bool MCmdDescription::execute( Creature* creature, const std::vector<std::string>& args ) {
  IOHandler* h = new TeditIOHandler( creature );
  h->getState()["vector"] = (void*)(new std::vector<std::string>(Regex::explode("\n",avatar()->medit()->identifiers().description())));
  h->getState()["name"] = (void*)(new std::string("Mob Description"));
  h->getState()["pointer"] = (void*)avatar()->medit()->identifiers().descriptionp();
  avatar()->pushIOHandler( h );
  return true;
}

MCmdInformation::MCmdInformation( void ) {
  name( "information" );
  level( BUILDER );
  addSyntax( 0, "" );
  brief( "Displays information about the Mob." );
  return;
}

bool MCmdInformation::execute( Creature* creature, const std::vector<std::string>& args ) {
  avatar()->send( Mob::getInformation( avatar()->medit() ) );
  return true;
}

MCmdKeywords::MCmdKeywords( void ) {
  name( "keywords" );
  level( BUILDER );
  addSyntax( -1, "<key1 key2 key3 ...>" );
  brief( "Updtes the Mobs keywords." );
  return;
}

bool MCmdKeywords::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::vector<std::string> keywords = Regex::explode( " ", args[0] );
  avatar()->medit()->identifiers().getKeywords().clear();
  for ( std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it ) {
    avatar()->medit()->identifiers().addKeyword( *it );
  }
  avatar()->send( "You've set the mob keywords to \"%s\".", avatar()->medit()->identifiers().getKeywordList().c_str() );
  return true;
}

MCmdLevel::MCmdLevel( void ) {
  name( "level" );
  level( BUILDER );
  addSyntax( 1, "<number>" );
  brief( "Updates the Mobs level." );
  addOptions( "level", "must be between 1 and 100, inclusive" );
  return;
}

bool MCmdLevel::execute( Creature* creature, const std::vector<std::string>& args ) {
  int level = estring(args[0]);
  if ( level > 0 && level < 101  ) {
    avatar()->medit()->level( level );
    avatar()->send( "You've set the mob level to %u.", avatar()->medit()->level() );
    return true;
  } else {
    avatar()->send( printSyntax() );
    return false;
  }
}

MCmdLongname::MCmdLongname( void ) {
  name( "longname" );
  level( BUILDER );
  addSyntax( -1, "<string>" );
  brief( "Updates the Mobs long name." );
  return;
}

bool MCmdLongname::execute( Creature* creature, const std::vector<std::string>& args ) {
  avatar()->medit()->identifiers().longname( args[0] );
  avatar()->send( "You've set the mob longname to \"%s\".", avatar()->medit()->identifiers().longname().c_str() );
  return true;
}

MCmdShortname::MCmdShortname( void ) {
  name( "shortname" );
  level( BUILDER );
  addSyntax( -1, "<string>" );
  brief( "Updates the Mobs short name." );
  return;
}

bool MCmdShortname::execute( Creature* creature, const std::vector<std::string>& args ) {
  avatar()->medit()->identifiers().shortname( args[0] );
  avatar()->send( "You've set the mob shortname to \"%s\".", avatar()->medit()->identifiers().shortname().c_str() );
  return true;
}
