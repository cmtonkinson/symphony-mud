/*
 * $Id: note.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_NOTE
#define H_SYMPHONY_NOTE

#include <string>
#include "mysql.h"

class Note {
  public:
    // constructors
    Note( const unsigned short& board, const unsigned long& author );
    Note( ROW row );
    ~Note( void );

    // public accessor methods
    void            ID( const unsigned long& ID )         { _ID = ID; }
    unsigned long   ID( void ) const                      { return _ID; }
    void            board( const unsigned short& board )  { _board = board; }
    unsigned short  board( void ) const                   { return _board; }
    void            author( const unsigned long& author ) { _author = author; }
    unsigned long   author( void ) const                  { return _author; }
    void            subject( const std::string& subject ) { _subject = subject; }
    std::string     subject( void ) const                 { return _subject; }
    void            body( const std::string& body )       { _body = body; }
    std::string     body( void ) const                    { return _body; }
    std::string*    bodyp( void )                         { return &_body; } // needs to be exposed for the text editor

    // general methods
    bool            save( void );
    bool            destroy( void );

  private:
    unsigned long   _ID;
    unsigned short  _board;
    unsigned long   _author;
    std::string     _subject;
    std::string     _body;
};

#endif // #ifndef H_SYMPHONY_AREA
