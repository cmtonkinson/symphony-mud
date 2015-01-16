/*
 * $Id: flagBank.cpp 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2009 Chris Tonkinson <cmtonkinson@gmail.com>
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

#include "flagBank.h"
#include "flagTable.h"

FlagBank::FlagBank(FlagInt bank):
    _bank(bank) {
  return;
}

FlagBank::FlagBank(const FlagBank& ref):
    _bank(ref.bank()) {
  return;
}

FlagBank::~FlagBank(void) {
  return;
}

std::string FlagBank::list(const FlagTable& table, bool decorative) const {
  std::string buf;
  const ValToName* map = decorative ? &table.decorativeName() : &table.valueToName();
  for (ValToName::const_iterator it = map->begin(); it != map->end(); ++it) {
    if (test(it->first)) {
      buf.append(it->second).append(1, ' ');
    }
  }
  if (!buf.empty()) {
    buf.resize(buf.size()-1);
  }
  return buf;
}
