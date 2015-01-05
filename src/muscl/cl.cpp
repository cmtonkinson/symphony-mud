/*
 * $Id: cl.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "cl.h"

ContextLayer::ContextLayer(void) {
  return;
}

ContextLayer::~ContextLayer(void) {
  return;
}

bool ContextLayer::isValidReference(const string& name) {
  return true;
}

bool ContextLayer::readable(const string& name) const {
  ContextNode* cn = resolve(name);
  if (cn && cn->readable()) return true;
  return false;
}

bool ContextLayer::writable(const string& name) const {
  ContextNode* cn = resolve(name);
  if (cn && cn->writable()) return true;
  return false;
}

bool ContextLayer::read(const string& name, Datum& value) {
  ContextNode* cn = resolve(name);
  if (cn) return cn->read(value);
  return false;
}

bool ContextLayer::write(const string& name, const Datum& value) {
  ContextNode* cn = resolve(name);
  if (cn) return cn->write(value);
  return false;
}

bool ContextLayer::add(const string& name, ContextNode* node) {
  ContextMap::const_iterator it;
  bool overwriting = false;
  if ((it = _objects.find(name)) != _objects.end()) {
    overwriting = true;
    remove(name);
  }
  _objects[name] = node;
  return overwriting;
}

bool ContextLayer::remove(const string& name) {
  ContextMap::const_iterator it;
  if ((it = _objects.find(name)) != _objects.end()) {
    delete it->second;
    _objects.erase(name);
    return true;
  }
  return false;
}

ContextNode* ContextLayer::resolve(const string& name) const {
  ContextMap::const_iterator it;
  if ((it = _objects.find(name)) != _objects.end()) return it->second;
  return NULL;
}
