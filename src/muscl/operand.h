/*
 * $Id: operand.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_BONES_OPERAND
#define H_SYMPHONY_BONES_OPERAND

#include <string>
#include "datum.h"

using std::string;

class Symbol;

class Operand {
  public:
    // constructors
    Operand(void);
    ~Operand(void);

    // public accessors
    void          address(const int& address)   { _address = address; }
    int           address(void) const           { return _address; }
    void          value(const Datum& value)     { _value = value; }
    Datum&        value(void)                   { return _value; }
    const Datum&  value(void) const             { return _value; }
    void          symbol(Symbol* symbol)        { _symbol = symbol; }
    Symbol*       symbol(void) const            { return _symbol; }


    // general methods
    bool            empty(void) const;
    void            print(void) const;
    string          serialize(void) const;
    static Operand  unserialize(const char* serialized, int start = 0);

  private:
    int     _address;
    Datum   _value;
    Symbol* _symbol;
};

#endif // End of inclusion guard
