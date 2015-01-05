/*
 * $Id: symboltable.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_SYMTAB
#define H_SYMPHONY_MUSCL_SYMTAB

#include <map>
#include <string>

class Symbol;

class SymbolTable {
  public:
    // Constructors...
    SymbolTable(void);
    SymbolTable(const std::string& name);
    ~SymbolTable(void);

    // Public accessor methods...
    void                                  name(const std::string& name) { _name = name; }
    std::string                           name(void) const              { return _name; }
    std::map<std::string,Symbol*>&        table(void)                   { return _table; }
    const std::map<std::string,Symbol*>&  table(void) const             { return _table; }

    // general methods...
    void        insert(Symbol* symbol);
    void        remove(Symbol* symbol);
    Symbol*     lookup(const std::string& name);
    std::string dump(void) const;

  private:
    std::string                   _name;
    std::map<std::string,Symbol*> _table;
};

#endif // !H_SYMPHONY_MUSCL_SYMTAB
