/*
 * $Id: commands-qs.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "commandTable.h"
#include "commandTable-default.h"
#include "commands-redit.h"
#include "display.h"
#include "io-handler.h"
#include "object-furniture.h"
#include "quoteTable.h"
#include "room.h"

#include "world.h"

/*
Cmd::Cmd( void ) {
  name( "" );
  level(  );
  addSyntax( 0, "" );
  brief( "" );
  return;
}

bool Cmd::execute( Creature* creature, const std::vector<std::string>& args ) {

  return true;
}
*/

CmdQuit::CmdQuit( void ) {
  name( "quit" );
  playerOnly( true );
  addSyntax( 0, "" );
  brief( "Log out of the game." );
  return;
}

bool CmdQuit::execute( Creature* creature, const std::vector<std::string>& args ) {
  Quote q = QuoteTable::Instance().getRandomQuote();
  if ( avatar()->save() ) {
    avatar()->send( "%s\n        -%s", q.second.c_str(), q.first.c_str() );
    avatar()->disconnected( true );
  } else {
    avatar()->send( "There was a problem saving your profile data.  Try again in a minute." );
    return false;
  }
  return true;
}

CmdReboot::CmdReboot( void ) {
  name( "reboot" );
  level( CREATOR );
  addSyntax( 1, "reboot" );
  brief( "Perform a hot-reboot (aka \"copyover\") of the game engine." );
}

bool CmdReboot::execute( Creature* creature, const std::vector<std::string>& args ) {
  if ( args[0] == "reboot" ) {
    World::Instance().save();
    return World::Instance().reboot( creature );
  } else {
    creature->send( printSyntax() );
    return false;
  }
}

CmdRedit::CmdRedit( void ) {
  name( "redit" );
  level( DEMIGOD );
  addSyntax( 0, "" );
  addSyntax( 2, "create <vnum>" );
  brief( "Invokes the Room Editor." );
  return;
}

bool CmdRedit::execute( Creature* creature, const std::vector<std::string>& args ) {
  RCmdCreate create;
  std::vector<std::string> create_args;
  CmdGoto go;
  std::vector<std::string> go_args;

  if ( args.size() == 2 ) {
    if ( args[0] == "create" ) {
      create_args.push_back( args[1] );
      // Everything looks clear; create the room...
      create.avatar( avatar() );
      if ( create.execute( avatar(), create_args ) ) {
        go_args.push_back( args[1] );
        go.avatar( avatar() );
        go.execute( avatar(), go_args );
        avatar()->pushIOHandler( new ReditIOHandler( avatar() ) );
      }
    } else {
      avatar()->send( printSyntax() );
      return false;
    }
  } else if ( args[0].empty() ) {
    // Check permissions...
    if ( !World::Instance().hasPermission( avatar()->room()->area(), avatar() ) ) {
      avatar()->send( "You don't have permissions to this room." );
      return false;
    }
    // Make sure no one else is editing the room...
    for ( std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it ) {
      if ( it->second->mode().number() == MODE_REDIT && it->second->room() == avatar()->room() ) {
        creature->send( "Sorry, %s is currently editing room %lu.", avatar()->seeName( ((Creature*)it->second) ).c_str(), avatar()->room()->vnum() );
        return false;
      }
    }
    // Looks good; send them to redit...
    avatar()->send( "You're editing Room %lu.", avatar()->room()->vnum() );
    avatar()->pushIOHandler( new ReditIOHandler( avatar() ) );
  } else {
    avatar()->send( printSyntax() );
    return false;
  }

  return true;
}

CmdRemove::CmdRemove( void ) {
  name( "remove" );
  addSyntax( 1, "<object>" );
  brief( "Removes the given piece of equipment." );
  return;
}

bool CmdRemove::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::list<Object*> objects = creature->equipment().searchObjects( args[0] );
  std::string error;

  if ( objects.empty() ) {
    creature->send( "You aren't wearing that." );
    return false;
  }

  for ( std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it ) {
    if ( creature->unwear( *it, error ) ) {
      creature->send( "You remove %s{x.\n", (*it)->identifiers().shortname().c_str() );
      creature->room()->send_cond( "$p removes $o.\n", creature, *it );
    } else {
      creature->send( error );
    }
  }

  return true;
}

CmdReply::CmdReply( void ) {
  name( "reply" );
  playerOnly( true );
  addSyntax( -1, "<message>" );
  brief( "Send an Out-Of-Character message to the last Player that sent you a 'tell.'" );
  return;
}

bool CmdReply::execute( Creature* creature, const std::vector<std::string>& args ) {
  Avatar* target = NULL;

  if ( avatar()->replyTo().empty() || ( target = World::Instance().findAvatar( avatar()->replyTo() ) ) == NULL ) {
    avatar()->send( "They aren't here." );
    return false;
  }

  if ( !target->channelFlags().test( CHANNEL_TELL ) ) {
    avatar()->send( "They're not receiving tells right now." );
    return false;
  }

  target->replyTo( avatar()->identifiers().shortname() );
  target->send( "%s replies to you, \"{Y%s{x\"", target->seeName( avatar() ).c_str(), args[0].c_str() );
  avatar()->send( "You reply to %s, \"{Y%s{x\"", avatar()->seeName( target ).c_str(), args[0].c_str() );

  return true;
}

CmdRestring::CmdRestring( void ) {
  name( "restring" );
  level( DEMIGOD );
  addSyntax( -3, "<object> shortname <string>" );
  addSyntax( -3, "<object> longname <string>" );
  addSyntax( -3, "<object> keywords <key1 key2 key3 ...>" );
  brief( "Alters the appearance of an Object." );
  return;
}

bool CmdRestring::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::vector<std::string> keywords;
  Object* object = NULL;

  if ( ( object = creature->inventory().searchSingleObject( args[0] ) ) == NULL ) {
    creature->send( "You don't have that." );
    return false;
  }

  if ( Regex::strPrefix( args[1], "shortname" ) ) {
    object->identifiers().shortname( args[2] );
    creature->send( "Object shortname reset to \"%s\".", object->identifiers().shortname().c_str() );
  } else if ( Regex::strPrefix( args[1], "longname" ) ) {
    object->identifiers().longname( args[2] );
    creature->send( "Object longname reset to \"%s\".", object->identifiers().longname().c_str() );
  } else if ( Regex::strPrefix( args[1], "keywords" ) ) {
    object->identifiers().getKeywords().clear();
    keywords = Regex::explode( " ", args[2] );
    for ( std::vector<std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it ) {
      object->identifiers().addKeyword( *it );
    }
    creature->send( "Object keywords reset to \"%s\".", object->identifiers().getKeywordList().c_str() );
  } else {
    creature->send( printSyntax() );
    return false;
  }

  return true;
}

CmdRlist::CmdRlist( void ) {
  name( "rlist" );
  level( DEMIGOD );
  addSyntax( 1, "<areaID>                       (list all Rooms in the area)" );
  addSyntax( 2, "<first vnum> <last vnum>       (list all Rooms in the vnum range)" );
  addSyntax( 1, "<keyword>                      (list all Rooms by name)" );
  addSyntax( 1, "/<regex>                       (list all Rooms matching the PCRE)" );
  return;
}

bool CmdRlist::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::vector<std::string> mutable_args( args );
  std::vector<Room*> rooms;
  Area* area = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[MAX_BUFFER];

  if ( mutable_args.size() == 1 ) {
    if ( Regex::match( "^[0-9]+$", mutable_args[0] ) ) {
      // We got an areaID...
      if ( ( area = World::Instance().findArea( estring(mutable_args[0]) ) ) == NULL ) {
        creature->send( "That area couldn't be found." );
        return false;
      }
      for ( std::map<unsigned long,Room*>::iterator r_it = area->rooms().begin(); r_it != area->rooms().end(); ++r_it ) {
        rooms.push_back( r_it->second );
      }
    } else {
      if ( mutable_args[0][0] == '/' ) {
        mutable_args[0].erase( 0, 1 );
        // This search is a regex...
        for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
          for ( std::map<unsigned long,Room*>::iterator r_it = (*a_it)->rooms().begin(); r_it != (*a_it)->rooms().end(); ++r_it ) {
            if ( Regex::match( mutable_args[0], r_it->second->name() ) ) {
              rooms.push_back( r_it->second );
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
          for ( std::map<unsigned long,Room*>::iterator r_it = (*a_it)->rooms().begin(); r_it != (*a_it)->rooms().end(); ++r_it ) {
            if ( Regex::lower(r_it->second->name()).find( search ) != std::string::npos ) {
              rooms.push_back( r_it->second );
            }
          }
        }
      }
    }
  } else if ( mutable_args.size() == 2 ) {
    /* We're looking for a vnum range here */
    // Grab our range values...
    low = estring(mutable_args[0]);
    high = estring(mutable_args[1]);
    // Check our range...
    if ( !high || low >= high ) {
      creature->send( "Invalid vnum range." );
      return false;
    }
    if ( low+400 < high ) {
      creature->send( "The maximum vnum range is 400." );
      return false;
    }
    // Grab the rooms...
    for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
      for ( std::map<unsigned long,Room*>::iterator r_it = (*a_it)->rooms().begin(); r_it != (*a_it)->rooms().end(); ++r_it ) {
        if ( r_it->second->vnum() >= low && r_it->second->vnum() <= high ) {
          rooms.push_back( r_it->second );
        }
      }
    }
  } else {
    creature->send( printSyntax() );
    return false;
  }

  if ( rooms.empty() ) {
    creature->send( "No matches for \"%s\"", mutable_args[0].c_str() );
    return false;
  }

  output.append( " ({carea{x) [{y room{x] {gname{x\n -------------------\n" );
  for ( std::vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it ) {
    sprintf( buffer, " ({c%4lu{x) [{y%5lu{x] {g%s{x\n", (*it)->area()->ID(), (*it)->vnum(), (*it)->name().c_str() );
    output.append( buffer );
  }

  creature->send( output );
  return true;
}

CmdSave::CmdSave( void ) {
  name( "save" );
  addSyntax( 0, "" );
  brief( "Saves your Player data." );
  return;
}

bool CmdSave::execute( Creature* creature, const std::vector<std::string>& args ) {
  if ( creature->save() ) {
    creature->send( "Information saved." );
    return true;
  } else {
    creature->send( "There was a problem saving your information." );
    return false;
  }
}

CmdSay::CmdSay( void ) {
  name( "say" );
  addSyntax( -1, "<message>" );
  brief( "In-Character speech." );
  return;
}

bool CmdSay::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string verb;
  switch ( args[0][args[0].size()-1] ) {
    case '?':
      creature->room()->send( "$p asks '{G$s{x'", creature, (void*)args[0].c_str() );
      creature->send( "You ask '{G%s{x'", args[0].c_str() );
      break;
    case '!':
      creature->room()->send( "$p exclaims '{G$s{x'", creature, (void*)args[0].c_str() );
      creature->send( "You exclaim '{G%s{x'", args[0].c_str() );
      break;
    default:
      creature->room()->send( "$p says '{G$s{x'", creature, (void*)args[0].c_str() );
      creature->send( "You say '{G%s{x'", args[0].c_str() );
      break;
  }
  return true;
}

CmdSedit::CmdSedit( void ) {
  name( "sedit" );
  level( GOD );
  addSyntax( 1, "<name>" );
  addSyntax( 2, "create <name>" );
  brief( "Invokes the Social Command Editor." );
  return;
}

bool CmdSedit::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string name;
  Command* command = NULL;

  if ( args.size() == 1 ) {
    name = args[0];
    if ( ( command = Commands::Instance().find( name ) ) == NULL ) {
      // does it exist?
      avatar()->send( "That social command couldn't be found." );
      return false;
    } else if ( !command->social() ) {
      // is it a social command?
      avatar()->send( "That command isn't a social." );
      return false;
    } else {
      // is anyone else editing it at the moment?
      for ( std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it ) {
        if ( a_it->second->mode().number() == MODE_SEDIT && a_it->second->sedit() == command ) {
          avatar()->send( "Sorry, %s is currently editing the \"%s\" command.", avatar()->seeName( ((Creature*)a_it->second) ).c_str(), command->name().c_str() );
          return false;
        }
      }
      // looks like we're good to go...
      avatar()->mode().set( MODE_SEDIT );
      avatar()->sedit( (SocialCommand*)command );
      avatar()->pushIOHandler( new SeditIOHandler( avatar() ) );
      avatar()->send( "You're editing social command \"%s\"", avatar()->sedit()->name().c_str() );
      return true;
    }
  }

  if ( args.size() == 2 ) {
    if ( args[0] != "create" ) {
      avatar()->send( printSyntax() );
      return false;
    } else {
      name = args[1];
      if ( ( command = Commands::Instance().find( name ) ) != NULL && command->name() == name ) {
        avatar()->send( "A command by that name already exists." );
        return false;
      } else {
        command = new SocialCommand( args[1], avatar()->ID() );
        avatar()->mode().set( MODE_SEDIT );
        avatar()->sedit( (SocialCommand*)command );
        avatar()->pushIOHandler( new SeditIOHandler( avatar() ) );
        avatar()->send( "Social command \"%s\" created successfully.", avatar()->sedit()->name().c_str() );
        return true;
      }
    }
  }

  return false;
}

CmdShutdown::CmdShutdown( void ) {
  name( "shutdown" );
  level( CREATOR );
  addSyntax( 1, "shutdown" );
  brief( "Shut down the game engine." );
  return;
}

bool CmdShutdown::execute( Creature* creature, const std::vector<std::string>& args ) {
  if ( args[0] == "shutdown" ) {
    World::Instance().exists( false );
    return true;
  } else {
    creature->send( printSyntax() );
    return false;
  }
}

CmdSit::CmdSit( void ) {
  name( "sit" );
  addSyntax( 0, "" );
  addSyntax( 2, "at <furniture>         (as in a bar or a table)" );
  addSyntax( 2, "on <furniture>         (as in a chair or a bed)" );
  brief( "Sit down." );
  return;
}

bool CmdSit::execute( Creature* creature, const std::vector<std::string>& args ) {
  Object* furniture = NULL;
  std::string error;

  if ( args.size() == 2 ) {
    // get the target furniture...
    if ( ( furniture = creature->room()->inventory().searchSingleObject( args[1] ) ) != NULL ) {
      if ( !furniture->isFurniture() ) {
        creature->send( "That's not furniture." );
        return false;
      }
    } else {
      creature->send( "You don't see that here." );
      return false;
    }
    // sit at it...
    if ( Regex::strPrefix( args[0], "at" ) ) {
      if ( creature->sit( error, furniture->furniture(), false ) ) {
        creature->send( "You sit down at %s.", furniture->identifiers().shortname().c_str() );
        creature->room()->send_cond( "$p sits down at $o.", creature, furniture );
      } else {
        creature->send( error );
        return false;
      }
    // sit on it...
    } else if ( Regex::strPrefix( args[0], "on" ) ) {
      if ( creature->sit( error, furniture->furniture(), true ) ) {
        creature->send( "You sit down on %s.", furniture->identifiers().shortname().c_str() );
        creature->room()->send_cond( "$p sits down on $o.", creature, furniture );
      } else {
        creature->send( error );
        return false;
      }
    // who knows what they were trying to do...
    } else {
      creature->send( printSyntax() );
    }
  } else {
    if ( !creature->sit( error ) ) {
      creature->send( error );
    } else {
      creature->send( "You sit down." );
      creature->room()->send_cond( "$p sits down.", creature );
      return true;
    }
  }
  return false;
}

CmdSocials::CmdSocials( void ) {
  name( "socials" );
  addSyntax( 0, "" );
  brief( "Displays the list of available social commands." );
  return;
}

bool CmdSocials::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::vector<std::string> names;
  for ( std::vector<Command*>::iterator it = Commands::Instance().commands().begin(); it != Commands::Instance().commands().end(); ++it ) {
    if ( (*it)->social() ) {
      names.push_back( (*it)->name() );
    }
  }
  creature->send( " \n--== {cSocial Commands ({C%u{c) {x==--\n", names.size() );
  creature->send( Display::formatColumns( names ) );
  return true;
}

CmdStand::CmdStand( void ) {
  name( "stand" );
  addSyntax( 0, "" );
  brief( "Stand up." );
  return;
}

bool CmdStand::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string error;
  if ( !creature->stand( error ) ) {
    creature->send( error );
    return false;
  }
  if ( creature->furniture() ) {
    creature->furniture( NULL );
    creature->furniture()->remove( creature );
  }
  creature->send( "You stand up." );
  creature->room()->send_cond( "$p stands up.", creature );
  return true;
}

CmdSummary::CmdSummary( void ) {
  name( "summary" );
  playerOnly( true );
  addSyntax( 0, "" );
  brief( "Displays your Player data." );
  return;
}

bool CmdSummary::execute( Creature* creature, const std::vector<std::string>& args ) {
  avatar()->send( "\n %s{x\n", avatar()->identifiers().shortname().c_str() );
  avatar()->send( "{w _______________________________________________________________________\n" );
  avatar()->send( "{w|\\_____________________________________________________________________/|\n" );
  avatar()->send( "{w||{xrace:   {g%-9s{x {w||{xstren: {G%2u{x/{g%2u{w ||                {w||{xlevel: {g%3u{w      ||\n", avatar()->race().string().c_str(), avatar()->strength(), avatar()->maxStrength(), avatar()->level() );
  avatar()->send( "{w||{xclass:  {g%-9s{x {w||{xdexte: {G%2u{x/{g%2u{w ||                {w||{xhp:   {G%4u{x/{g%-4u{w ||\n", avatar()->pClass().string().c_str(), avatar()->dexterity(), avatar()->maxDexterity(), avatar()->health(), avatar()->maxHealth() );
  avatar()->send( "{w||{xgender: {g%-7s{x   {w||{xconst: {G%2u{x/{g%2u{w ||                {w||{xmana: {C%4u{x/{c%-4u{w ||\n", avatar()->gender().string().c_str(), avatar()->constitution(), avatar()->maxConstitution(), avatar()->mana(), avatar()->maxMana() );
  avatar()->send( "{w||{xage:    {g%-3u{x       {w||{xintel: {G%2u{x/{g%2u{w ||{xarmor: {g%-4d{w     ||{xmove: {M%4u{x/{m%-4u{w ||\n", avatar()->age(), avatar()->intelligence(), avatar()->maxIntelligence(), avatar()->armor(), avatar()->movement(), avatar()->maxMovement() );
  avatar()->send( "{w||{xhand:   {g%-5s{x     {w||{xfocus: {G%2u{x/{g%2u{w || {x-bash:   {g%-4d{w  ||{xexp: {g%-9u{w  ||\n", ((avatar()->hand() == WEARLOC_HOLD_R) ? "right" : "left"), avatar()->focus(), avatar()->maxFocus(), avatar()->bash(), avatar()->exp() );
  avatar()->send( "{w||{xheight: {g%-9s{x {w||{xcreat: {G%2u{x/{g%2u{w || {x-slash:  {g%-4d{w  ||{xtnl: {g%-7u{w    ||\n", "-", avatar()->creativity(), avatar()->maxCreativity(), avatar()->slash(), avatar()->tnl() );
  avatar()->send( "{w||{xweight: {g%-9s{x {w||{xchari: {G%2u{x/{g%2u{w || {x-pierce: {g%-4d{w  ||{xpracs: {g%-4u{w     ||\n", "-", avatar()->charisma(), avatar()->maxCharisma(), avatar()->pierce(), avatar()->practices() );
  avatar()->send( "{w||{xtotem:  {g%-9s{x {w||{xluck:  {G%2u{x/{g%2u{w || {x-exotic: {g%-4d{w  ||{xpoints: {g%-4u{w    ||\n", "-", avatar()->luck(), avatar()->maxLuck(), avatar()->exotic(), avatar()->points() );
  avatar()->send( "{w||__________________||_____________||__________________||______________||\n" );
  avatar()->send( "{w||_____________________________________________________________________||\n" );
  avatar()->send( "{w||{xitems: {G%3u{x/{g%-3u{w    ||{xcoins: {W%9u{x/{y%-9u{w     {w||{xCT: {g%-6u{w      ||\n", avatar()->inventory().objectList().size(), avatar()->inventory().objectList().size(), avatar()->silver(), avatar()->gold(), 0 );
  avatar()->send( "{w||{xweight: {g%-5u{w     ||{xbank:  {W%9u{x/{y%-9u{w     {w||                ||\n", 0, avatar()->bankSilver(), avatar()->bankGold() );
  avatar()->send( "{w||__________________||_______________________________||________________||\n" );
  avatar()->send( "{w|/_____________________________________________________________________\\|{x\n" );
  avatar()->send( "  You've been logged on for %s.\n", avatar()->stringLoggedOn().c_str() );
  if ( avatar()->composing().number() ) {
    avatar()->send( "  You are currently composing a %s.\n", avatar()->composing().string().c_str() );
  }
  avatar()->send( "  You are %s", avatar()->position().string().c_str() );
  if ( avatar()->action().number() ) {
    avatar()->send( "and %s", avatar()->action().string().c_str() );
  }
  avatar()->send( ".\n" );
  avatar()->send( "  Your title is: %s{x\n", avatar()->title() );
  if ( avatar()->whoFlags().any() ) {
    avatar()->send( "  You're marked as: %s\n", avatar()->listWhoFlags().c_str() );
  }
  return true;
}

CmdSummon::CmdSummon( void ) {
  name( "summon" );
  level( DEMIGOD );
  addSyntax( 1, "<player>" );
  addSyntax( 1, "<mob>" );
  brief( "Transports the target to the current room." );
  return;
}

bool CmdSummon::execute( Creature* creature, const std::vector<std::string>& args ) {
  Creature* target = NULL;
  Room* from = NULL;
  Room* to = avatar()->room();
  CmdLook look;
  std::vector<std::string> look_args( 1 );

  // Aquire target...
  if ( ( target = World::Instance().findCreature( args[0] ) ) == NULL ) {
    avatar()->send( "They're not around at the moment." );
    return false;
  }

  // Check permissions...
  if ( !avatar()->canAlter( target ) ) {
    avatar()->send( "You can't summon %s.\n", target->identifiers().shortname().c_str() );
    target->send( "%s has attempted to summon you.", avatar()->identifiers().shortname().c_str() );
    return false;
  }

  // Are they already here?
  if ( target->room() == avatar()->room() ) {
    avatar()->send( "%s is already here.\n", target->identifiers().shortname().c_str() );
    return false;
  }

  // Summon...
  from = target->room();
  target->send( "You've been summoned by %s.\n", target->seeName( avatar() ).c_str() );
  from->send_cond( "$p disappears in small puff of smoke.", target );
  from->remove( target );
  to->add( target );
  target->room( to );
  to->send_cond( "$p appears with a small \"pop!\"", target );
  look.avatar( avatar() );
  look.execute( target, look_args );
  return true;
}
