/*
 * $Id: board.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_BOARD
#define H_SYMPHONY_BOARD

#include <map>
#include "note.h"

// board numbers
#define BOARD_GENERAL   1
#define BOARD_CHANGES   2
#define BOARD_ADMIN     3
#define BOARD_NEWS      4
#define BOARD_IMMORTAL  5
#define BOARD_BUILDING  6

class Board {
  public:
    // constructors
    Board( const unsigned short& number );
    ~Board( void );

    // public accessors...
    void                                  number( const unsigned short& number )    { _number = number; }
    unsigned short                        number( void ) const                      { return _number; }
    std::map<unsigned long,Note*>&        notes( void )                             { return _notes; }
    const std::map<unsigned long,Note*>&  notes( void ) const                       { return _notes; }

    // general methods...
    const char*                           name( void ) const;
    bool                                  load( void );
    void                                  save( void );

  private:
    // data...
    unsigned short                        _number;
    std::map<unsigned long,Note*>         _notes;
};

#endif // #ifndef H_SYMPHONY_AREA
