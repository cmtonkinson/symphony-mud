/*
 * $Id: container.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "container.h"
#include "estring.h"
#include "object.h"

Container::Container( void ) {
  return;
}

Container::~Container( void ) {
  return;
}

std::vector<std::string> Container::parseQuery( const std::string& q, int& multiplier, int& index ) {
  std::string query = Regex::trim( Regex::lower( Regex::strip( q, '\'' ) ) );
  std::vector<std::string> keywords;
  size_t pos = 0;

  if ( query == "all" ) {
    // return all items
    multiplier = CONTAINER_ALL;
    index = CONTAINER_NONE;
  } else if ( Regex::strPrefix( "all*", query ) ) {
    // return all of the matched items
    multiplier = CONTAINER_ALL_W_KEYWORDS;
    index = CONTAINER_NONE;
    keywords = Regex::explode( " ", query.substr( 4 ), true );
  } else if ( ( pos = query.find( '*' ) ) != std::string::npos ) {
    // return a certain number of the matched items
    multiplier = estring( query.substr( 0, pos ) );
    index = CONTAINER_NONE;
    keywords = Regex::explode( " ", query.substr( pos+1 ), true );
  } else if ( ( pos = query.find( '#' ) ) != std::string::npos ) {
    // return a specific offset of the matched items
    multiplier = 1;
    index = estring( query.substr( pos+1 ) );
    keywords = Regex::explode( " ", query.substr( 0, pos ), true );
  } else {
    // we only want one item - plain match
    multiplier = 1;
    index = CONTAINER_NONE;
    keywords = Regex::explode( " ", query, true );
  }

  return keywords;
}

std::list<Object*> Container::search( const std::list<Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index ) {
  int counter = 0;
  std::list<Object*> matches;

  /* return everything */
  if ( multiplier == CONTAINER_ALL && index == CONTAINER_NONE ) {
    matches.insert( matches.end(), objects.begin(), objects.end() );
  /* return all of the matched items */
  } else if ( multiplier == CONTAINER_ALL_W_KEYWORDS && index == CONTAINER_NONE ) {
    for ( std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( *it, keywords ) ) {
        matches.push_back( *it );
      }
    }
  /* return only so many of the matched items */
  } else if ( multiplier > 0 && index == CONTAINER_NONE ) {
    for ( std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( *it, keywords ) ) {
        matches.push_back( *it );
        if ( ++counter >= multiplier ) {
          break;
        }
      }
    }
  /* return a specific matched item */
  } else if ( index > 0 && multiplier == 1 ) {
    for ( std::list<Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( *it, keywords ) && ++counter == index ) {
        matches.push_back( *it );
        break;
      }
    }
  }

  return matches;
}

std::list<Object*> Container::search( const std::map<int,Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index ) {
  int counter = 0;
  std::list<Object*> matches;

  /* return everything */
  if ( multiplier == CONTAINER_ALL && index == CONTAINER_NONE ) {
    for ( std::map<int,Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      matches.push_back( it->second );
    }
  /* return all of the matched items */
  } else if ( multiplier == CONTAINER_ALL_W_KEYWORDS && index == CONTAINER_NONE ) {
    for ( std::map<int,Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( it->second, keywords ) ) {
        matches.push_back( it->second );
      }
    }
  /* return only so many of the matched items */
  } else if ( multiplier > 0 && index == CONTAINER_NONE ) {
    for ( std::map<int,Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( it->second, keywords ) ) {
        matches.push_back( it->second );
        if ( ++counter >= multiplier ) {
          break;
        }
      }
    }
  /* return a specific matched item */
  } else if ( index > 0 && multiplier == 1 ) {
    for ( std::map<int,Object*>::const_iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( commonSearch( it->second, keywords ) && ++counter == index ) {
        matches.push_back( it->second );
        break;
      }
    }
  }

  return matches;
}

bool Container::commonSearch( Object* object, const std::vector<std::string>& keywords ) {
  for ( std::vector<std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it ) {
    if ( !object->identifiers().matchesKeyword( *it ) ) {
      return false;
    }
  }
  return true;
}
