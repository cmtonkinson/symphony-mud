/*
 * $Id: enum.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#include "enum.h"
#include "enumTable.h"

Enum::Enum(EnumInt value):
    _value(value) {
  return;
}

Enum::~Enum(void) {
  return;
}

void Enum::operator = (const Enum& ref) {
  _value = ref.number();
  _table = ref._table;
  return;
}

EnumInt Enum::number(void) const {
  return _value;
}

std::string Enum::string(void) const {
  return _table->get(_value);
}

void Enum::set(const EnumInt& value) {
  _value = value;
  return;
}

void Enum::set(const std::string& value) {
  _value = _table->get(value);
  return;
}
