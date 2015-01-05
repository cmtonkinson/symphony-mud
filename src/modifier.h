/*
 * $Id: modifier.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MODIFIER
#define H_SYMPHONY_MODIFIER

class Modifier {
  public:
    // Constructors...
    Modifier( void );
    Modifier( const Modifier& ref );
    Modifier( const unsigned short& attribute, const int& magnitude );
    ~Modifier( void );

    // Public accessor methods...
    void            attribute( const unsigned short& attribute )  { _attribute = attribute; }
    unsigned short  attribute( void ) const                       { return _attribute; }
    void            magnitude( const int& magnitude )             { _magnitude = magnitude; }
    int             magnitude( void ) const                       { return _magnitude; }

  private:
    unsigned short  _attribute;
    int             _magnitude;
};

#endif // #ifndef H_SYMPHONY_MODIFIER
