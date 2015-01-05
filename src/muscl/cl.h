/*
 * $Id: cl.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_CL
#define SYMPHONY_CL

#include <map>
#include <string>
#include "datum.h"
#include "cl-node.h"

using std::map;
using std::string;

class ContextLayer {
  public:
    typedef map<string,ContextNode*> ContextMap;

    ContextLayer(void);
    ~ContextLayer(void);

    bool  isValidReference(const string& name);

    bool  readable(const string& name) const;
    bool  writable(const string& name) const;

    bool  read(const string& name, Datum& value);
    bool  write(const string& name, const Datum& value);

    bool          add(const string& name, ContextNode* node);
    bool          remove(const string& name);
    ContextNode*  resolve(const string& name) const;

  private:
    ContextMap  _objects;
};

#endif // #ifndef SYMPHONY_CL
