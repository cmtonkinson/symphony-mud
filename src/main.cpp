/*
 * $Id: main.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#include <stdio.h>
#include "world.h"
#include "display.h"

int main( int argc, char* argv[], char* envp[] ) {
  std::string arguments;
  std::string descriptor;

  // FlagBank b;

  // b.set( ADMIN_HEADBUILDER );
  // printf( "testHB %s\n", b.test( ADMIN_HEADBUILDER ) ? "true" : "false" );
  // printf( "value: %lu\n", b.value() );

  // return 0;

  try {
    srand( time( NULL ) );

    for ( int i = 0; i < argc; ++i ) {
      arguments.append( argv[i] ).append( " " );
    }

    if ( Regex::match( "-copyover\\s+-fd=(\\d+)", arguments, descriptor ) ) {
        World::Instance().copyover( true );
        World::Instance().recover( estring( descriptor ) );
    } else {
      World::Instance().startup();
    }

  } catch ( SocketException e ) {
    fprintf( stderr, "%s: SocketException (error: %s)\n", "top-level", e.getError().c_str() );
    return false;
  } catch ( MysqlException e ) {
    fprintf( stderr, "%s: MysqlException (message: %s)\n", "top-level", e.getMessage().c_str() );
    return false;
  } catch ( RegexException e ) {
    fprintf( stderr, "%s: RegexException (message: %s)\n", "top-level", e.getMessage().c_str() );
    return false;
  } catch ( std::string s ) {
    fprintf( stderr, "%s: std::string (%s)\n", "top-level", s.c_str() );
    return false;
  } catch ( std::exception e ) {
    fprintf( stderr, "%s: std::exception (what: %s)\n", "top-level", e.what() );
    return false;
  } catch ( ... ) {
    fprintf( stderr, "%s: something failed - good luck!\n", "top-level" );
    return false;
  }

  return EXIT_NORMAL;
}
