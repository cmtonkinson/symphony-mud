/*
 * $Id: exit.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_EXIT
#define H_SYMPHONY_EXIT

#include "enumTable.h"
#include "flagTable.h"
#include "mysql.h"

class Room;

class Exit {
  public:
    Exit(Room* target, ROW& row);
    Exit(Room* room, Room* target, const unsigned short& direction);
    ~Exit(void);

    // Public accessor methods...
    void                  ID(const unsigned long& ID)                 { _ID = ID; }
    unsigned long         ID(void) const                              { return _ID; }
    void                  target(Room* target)                        { _target = target; }
    Room*                 target(void)                                { return _target; }
    Direction&            direction(void)                             { return _direction; }
    const Direction&      direction(void) const                       { return _direction; }
    FlagBank&             flags(void)                                 { return _flags; }
    const FlagBank&       flags(void) const                           { return _flags; }
    void                  key(const unsigned long& key)               { _key = key; }
    unsigned long         key(void) const                             { return _key; }

    // General methods...
    void                  flag(const unsigned long& flag, const bool& value, bool stop = false);
    bool                  flag(const unsigned long& flag)             { return _flags.test(flag); }
    void                  save(void);
    void                  destroy(void);

    // Statics...
    static unsigned short inverse(const unsigned short& direction);
    static const char*    name(const unsigned short& direction);
    static const char*    inverseName(const unsigned short& direction);
    static unsigned short string2dir(const std::string& str);
    static unsigned short string2inverse(const std::string& str);
    static const char*    getFlagName(const unsigned long& flag);
    static unsigned long  getFlagType(const std::string& flag);

  private:
    unsigned long   _ID;
    Room*           _target;
    Direction       _direction;
    FlagBank        _flags;
    unsigned long   _key;
};

#endif // #ifndef H_SYMPHONY_EXIT
