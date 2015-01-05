/*
 * $Id: symbol.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "datum.h"
#include "symbol.h"

Symbol::Symbol(void):
    _type(SYMBOL_NEW),
    _symlist(NULL),
    _code(NULL),
    _offset(-1),
    _line(-1) {
  return;
}

Symbol::Symbol(const std::string& name, unsigned type, int line):
    _type(type),
    _name(name),
    _symlist(NULL),
    _code(NULL),
    _offset(-1),
    _line(line) {
  return;
}

Symbol::~Symbol(void) {
  return;
}

const char* Symbol::stringType(void) const {
  switch (type()) {
    case SYMBOL_NEW:    return "new";
    case SYMBOL_CONST:  return "constant";
    case SYMBOL_VAR:    return "variable";
    case SYMBOL_FUNC:   return "function";
    default:            return "undefined";
  }
}

std::string Symbol::dump(void) const {
  std::string buf("'");
  buf.append(name()).append("' (").append(stringType()).append(")");
  return buf;
}
