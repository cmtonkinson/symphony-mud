/*
 * $Id: symboltable.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "symbol.h"
#include "symboltable.h"

SymbolTable::SymbolTable(void) {
  return;
}

SymbolTable::SymbolTable(const std::string& name):
    _name(name) {
  return;
}

SymbolTable::~SymbolTable(void) {
  for (std::map<std::string,Symbol*>::iterator it = table().begin(); it != table().end(); ++it) delete it->second;
  return;
}

void SymbolTable::insert(Symbol* symbol) { // public
  table()[symbol->name()] = symbol;
  return;
}

void SymbolTable::remove(Symbol* symbol) { // public
  table().erase(symbol->name());
  return;
}

Symbol* SymbolTable::lookup(const std::string& name) { // public
  std::map<std::string,Symbol*>::iterator it;
  if ((it = table().find(name)) == table().end()) return NULL;
  else return it->second;
}

std::string SymbolTable::dump(void) const {
  std::string buf;
  for (std::map<std::string,Symbol*>::const_iterator it = table().begin(); it != table().end(); ++it) {
    buf.append("  ").append(it->second->dump()).append(1, '\n');
  }
  return buf;
}
