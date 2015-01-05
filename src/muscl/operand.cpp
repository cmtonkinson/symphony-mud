/*
 * $Id: operand.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "operand.h"

#include <sstream>
#include <stdio.h>
#include "datum.h"
#include "symbol.h"

Operand::Operand(void):
    _address(0),
    _symbol(NULL) {
  return;
}

Operand::~Operand(void) {
  return;
}

bool Operand::empty(void) const {
  if (address() > 0 || value().data() || symbol()) return false;
  else return true;
}

void Operand::print(void) const {
  if (address() > 0) {
    printf(" @%-20d", address());
  } else if (symbol()) {
    printf(" *%-20s", symbol()->name().c_str());
  } else if (value().data()) {
    printf(" %-21s", value().serialize().c_str());
  } else {
    printf("                      ");
  }
  return;
}

string Operand::serialize(void) const {
  string dest;
  std::ostringstream buffer;
  if (address() > 0) buffer << address();
  else if (value().data()) buffer << value().serialize();
  return dest.append(buffer.str());
}

Operand Operand::unserialize(const char* serialized, int start) {
  Operand dest;
  return dest;
}
