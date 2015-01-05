/*
 * $Id: estring.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_ESTRING
#define H_ESTRING

#include <cstdlib>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>

// Handy conversion to have (gives strings the stream insertion operator over various data types)...
template <class type> inline std::string& operator << ( std::string& dest, const type& t ) {
  std::ostringstream buffer;
  buffer << t;
  return dest.append(buffer.str());
}

class extended_string: public std::string {
  public:
    extended_string( void )                             { return; }
    extended_string( const extended_string& ref )       { this->assign( ref.data() ); }
    extended_string( const char* cs )                   { this->assign( cs ); return; }
    extended_string( const std::string& s )             { this->assign( s ); return; }
    extended_string( const signed short int& value )    { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return; }
    extended_string( const unsigned short int& value )  { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return; }
    extended_string( const signed int& value )          { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return; }
    extended_string( const unsigned int& value )        { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return; }
    extended_string( const signed long int& value )     { char buffer[32]; sprintf( buffer, "%ld", value ); this->assign( buffer ); return; }
    extended_string( const unsigned long int& value )   { char buffer[32]; sprintf( buffer, "%ld", value ); this->assign( buffer ); return; }
    extended_string( const float& value )               { char buffer[32]; sprintf( buffer, "%f", value ); this->assign( buffer ); return; }
    extended_string( const double& value )              { char buffer[32]; sprintf( buffer, "%f", value ); this->assign( buffer ); return; }
    extended_string( const char& value )                { this->assign( 1, value ); return; }
    ~extended_string( void )                            { return; }

    extended_string& operator = ( const std::string& value )        { this->assign( value ); return *this; }
    extended_string& operator = ( const signed short int& value )   { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const unsigned short int& value ) { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const signed int& value )         { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const unsigned int& value )       { char buffer[32]; sprintf( buffer, "%d", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const signed long int& value )    { char buffer[32]; sprintf( buffer, "%ld", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const unsigned long int& value )  { char buffer[32]; sprintf( buffer, "%ld", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const float& value )              { char buffer[32]; sprintf( buffer, "%f", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const double& value )             { char buffer[32]; sprintf( buffer, "%f", value ); this->assign( buffer ); return *this; }
    extended_string& operator = ( const char& value )               { this->assign( 1, value ); return *this; }

    operator signed short int   ( void ) const { return atol( this->c_str() ); }
    operator unsigned short int ( void ) const { return atol( this->c_str() ); }
    operator signed int         ( void ) const { return atoi( this->c_str() ); }
    operator unsigned int       ( void ) const { return atoi( this->c_str() ); }
    operator signed long int    ( void ) const { return atol( this->c_str() ); }
    operator unsigned long int  ( void ) const { return atol( this->c_str() ); }
    operator float              ( void ) const { return atof( this->c_str() ); }
    operator double             ( void ) const { return atof( this->c_str() ); }
    operator char               ( void ) const { return this->c_str()[0]; }
    operator const char*        ( void ) const { return this->c_str(); }
};

template <class key_type, class val_type> class extended_map: public std::map<key_type,val_type> {
  public:
    bool operator = ( const extended_map<key_type,val_type>& src ) {
      this->std::map<key_type,val_type>::operator = ( src );
      return !this->empty();
    }
};

typedef extended_string estring;
typedef extended_map<std::string,extended_string> emap;

#endif // #ifndef H_ESTRING
