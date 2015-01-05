/*
 * $Id: program.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_PROGRAM
#define SYMPHONY_PROGRAM

#include <string>
#include <vector>
#include "record.h"

using std::string;
using std::vector;

typedef vector<Record> CodeVect;

/*
 * This class handles a few important abstractions:
 *   1) Keeps compiled code organized together with its source
 *   2) Manages program metadata
 *   3) Keeps file backings of all data
 */
class Program {
  public:
    Program(void);
    Program(const string& name);
    ~Program(void);

    string                name(void) const              { return _name; }
    bool                  source(const string& source);
    string                source(void) const            { return _source; }
    CodeVect const* const instructions(void) const      { return &_instructions; }

    bool                  compile(string& feedback);

  private:
    string    _name;
    string    _source;
    CodeVect  _instructions;
};

#endif // #ifndef SYMPHONY_PROGRAM
