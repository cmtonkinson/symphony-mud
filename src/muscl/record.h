/*
 * $Id: record.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_BONES_RECORD
#define H_SYMPHONY_BONES_RECORD

#include "operand.h"

class Datum;
class Symbol;

class Record {
  public:
    // constructors
    Record(void);
    Record(const int& opcode);
    ~Record(void);

    // general methods
    void        print(void) const;
    const char* opcodeToString(void) const;

    // public accessors
    void            number(const int& number)   { _number = number; }
    int             number(void) const          { return _number; }
    void            opcode(const int& opcode)   { _opcode = opcode; }
    int             opcode(void) const          { return _opcode; }
    Operand&        left(void)                  { return _left; }
    const Operand&  left(void) const            { return _left; }
    Operand&        right(void)                 { return _right; }
    const Operand&  right(void) const           { return _right; }

  private:
    int     _number;
    int     _opcode;
    Operand _left;
    Operand _right;
};

#endif // End of inclusion guard
