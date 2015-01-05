/*
 * $Id: identifiers.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "identifiers.h"
#include "regex.h"

Identifiers::Identifiers( void ) {
  return;
}

Identifiers::Identifiers( const Identifiers& ref ) {
  shortname( ref.shortname() );
  longname( ref.longname() );
  description( ref.description() );
  setKeywords( ref.getKeywords() );
  return;
}

Identifiers::~Identifiers( void ) {
  getKeywords().clear();
  return;
}

std::string Identifiers::getKeywordList( std::string glue ) const {
  return Regex::implode( glue, getKeywords() );
}

void Identifiers::addKeyword( const std::string& keyword ) {
  std::string foo = ColorString( keyword ).stripColor();
  Regex::replace( "[^a-z0-9]+", "", foo );
  getKeywords().insert( Regex::lower( foo ) );
  return;
}

void Identifiers::removeKeyword( const std::string& keyword ) {
  getKeywords().erase( Regex::lower( ColorString( keyword ).stripColor() ) );
  return;
}

bool Identifiers::matchesKeyword( const std::string& keyword ) const {
  std::string foo = Regex::lower( ColorString( keyword ).stripColor() );
  for ( std::set<std::string>::const_iterator it = getKeywords().begin(); it != getKeywords().end(); ++it ) {
    if ( Regex::strPrefix( foo, *it ) ) {
      return true;
    }
  }
  return false;
}

std::string Identifiers::serializeKeywords( void ) const {
  return Regex::implode( "|", getKeywords() );
}

void Identifiers::unserializeKeywords( const std::string& serialization ) {
  std::vector<std::string> keywords = Regex::explode( "|", serialization );
  for ( std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it ) {
    addKeyword( *it );
  }
  return;
}
