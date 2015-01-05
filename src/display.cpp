/*
 * $Id: display.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#include <math.h>
#include "creature.h"
#include "display.h"
#include "exit.h"
#include "object.h"

Display::Display( void ) {
  return;
}

Display::~Display( void ) {
  return;
}

std::string Display::formatColumns( const std::vector<std::string>& items ) {
  unsigned long cols = 6;
  unsigned long column_width = 13;
  unsigned long rows = items.size()/cols+1;
  unsigned long offset = 0;
  unsigned long size = items.size();
  char* buffer = (char*)calloc( column_width, sizeof(char) );
  std::string foo;
  std::string dest;

  // Loop through rows and columns and print each item in the list...
  for ( unsigned long row = 0; row < rows; ++row ) {
    for ( unsigned long column = 0; column < cols; ++column ) {
      // Calculate our list offset for the current row/column...
      offset = column*rows+row;
      if ( offset < size ) {
        // The offset is a valid list element...
        foo = items[offset];
      } else {
        // The offset is out of bounds (only happens in rightmost column)...
        foo.clear();
      }
      // Copy the data...
      sprintf( buffer, "%-13s{x", foo.substr( 0, column_width ).c_str() );
      dest.append( buffer );
    }
    // Newline after every row but the last...
    if ( row < rows-1 ) {
      dest.append( 1, '\n' );
    }
  }

  free( buffer );
  return dest;
}

std::string Display::formatAction( const char* format, Creature* creature, void* arg1, void* arg2, Creature* withRespectTo ) {
  std::string input( format );
  std::string message;

  for ( std::string::iterator it = input.begin(); it != input.end(); ++it ) {
    if ( *it == '$' ) {
      switch ( *(it+1) ) {
        // arbitrary strings
        case 's': message.append( arg1 ? (const char*)arg1 : "[s]" );                                             break;
        case 'S': message.append( arg2 ? (const char*)arg2 : "[S]" );                                             break;
        // the 'actor'
        case 'p': message.append( withRespectTo ? withRespectTo->seeName(creature) : "[p]" );                     break;
        // arbitrary creatures
        case 'c': message.append( withRespectTo ? withRespectTo->seeName((Creature*)arg1) : "[c]" );              break;
        case 'C': message.append( withRespectTo ? withRespectTo->seeName((Creature*)arg2) : "[C]" );              break;
        // reflexive pronouns
        case 'f': message.append( withRespectTo ? withRespectTo->seeReflexivePronoun((Creature*)arg1) : "[f]" );  break;
        case 'F': message.append( withRespectTo ? withRespectTo->seeReflexivePronoun((Creature*)arg2) : "[F]" );  break;
        // object pronouns
        case 'b': message.append( withRespectTo ? withRespectTo->seeObjectPronoun((Creature*)arg1) : "[b]" );     break;
        case 'B': message.append( withRespectTo ? withRespectTo->seeObjectPronoun((Creature*)arg2) : "[B]" );     break;
        // possessive pronouns
        case 'r': message.append( withRespectTo ? withRespectTo->seePosessivePronoun((Creature*)arg1) : "[r]" );  break;
        case 'R': message.append( withRespectTo ? withRespectTo->seePosessivePronoun((Creature*)arg2) : "[R]" );  break;
        // objects
        case 'o': message.append( arg1 ? ((Object*)arg1)->identifiers().shortname() : "[o]" );                    break;
        case 'O': message.append( arg2 ? ((Object*)arg2)->identifiers().shortname() : "[O]" );                    break;
        // exits
        case 'e': message.append( arg1 ? Exit::name( ((Exit*)arg1)->direction().number() ) : "[e]" );             break;
        case 'E': message.append( arg2 ? Exit::name( ((Exit*)arg2)->direction().number() ) : "[E]" );             break;
        // exit inverses
        case 'n': message.append( arg1 ? Exit::inverseName(((Exit*)arg1)->direction().number()) : "[n]" );        break;
        case 'N': message.append( arg2 ? Exit::inverseName(((Exit*)arg2)->direction().number()) : "[N]" );        break;
        // huh?
        default:  message.append( "[$]" ); break;
      }
      message.append( "{x" );
      ++it;
    } else {
      message.append( 1, *it );
    }
  }
  message[0] = toupper(message[0]);       // proper mechanics
  Regex::replace( "s's", "s'", message ); // fix for posessives ending in 's'
  return message;
}
