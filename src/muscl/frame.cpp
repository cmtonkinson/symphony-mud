/*
 * $Id: frame.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "frame.h"

#include "datum.h"

Frame::Frame(void):
    _comparison(false),
    _returnAddress(0) {
  return;
}

Frame::~Frame(void) {
  return;
}

IterPair& Frame::get_pair(const long& address) {
  IterMap::iterator it;
  if ((it = iterators().find(address)) == iterators().end()) {
    throw std::string("Frame::get_pair() failed to find an appropriate binding.");
  }
  return it->second;
}

const Datum& Frame::get_iter(const long& address) {
  return *get_pair(address).second;
}

void Frame::iter_new(const long& address, const Datum& thelist) {
  Datum* copy = NULL;
  if (iterators().find(address) != iterators().end()) {
    throw std::string("Frame::iter_new() attempted to re-bind an iterator.");
  }
  copy = new Datum(thelist);
  copy->make_list();
  iterators().insert(std::make_pair(address, std::make_pair((DList*)copy->data(), ((DList*)copy->data())->begin())));
  return;
}

void Frame::iter_inc(const long& address) {
  ++get_pair(address).second;
  return;
}

bool Frame::iter_end(const long& address) {
  IterPair& pair = get_pair(address);
  if (pair.second == pair.first->end()) {
    return true;
  } else {
    return false;
  }
}

void Frame::iter_kill(const long& address) {
  if (iterators().find(address) == iterators().end()) {
    throw std::string("Frame::iter_kill() failed to find an approrpriate binding.");
  }
  iterators().erase(address);
  return;
}
