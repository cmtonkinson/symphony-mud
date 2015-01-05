/*
 * $Id: color-string.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_COLOR_STRING
#define H_SYMPHONY_COLOR_STRING

#include <stdio.h>
#include <stdlib.h>
#include <string>

class ColorString: public std::string {
  public:
    // constructors
    ColorString( void );
    ColorString( const std::string& ref );
    ColorString( const ColorString& ref );
    ~ColorString( void );

    // operators
    std::string& operator = ( const std::string& ref );

    /* return the length of the string ignoring color codes */
    size_t      effectiveLength( void ) const;
    /* return a substring ignoring color codes */
    std::string effectiveSubstr( size_t pos = 0, size_t n = npos ) const;
    /* return a string sans color codes */
    std::string stripColor( void ) const;
    /* return a string where color codes have been cast to the `\033[` format */
    std::string interpretColor( void ) const;

    // static methods
    static const char*  getColorCode( const char& c );
};

#endif // !H_SYMPHONY_COLOR_STRING
