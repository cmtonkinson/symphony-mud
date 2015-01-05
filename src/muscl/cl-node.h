/*
 * $Id: cl-node.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_CL_NODE
#define SYMPHONY_CL_NODE

#include <string>
#include "datum.h"

#define CL_PERM_READ  4
#define CL_PERM_WRITE 2

using std::string;

class ContextNode {
  public:
    ContextNode(const unsigned short& permission):
        _permission(permission) {
      return;
    }
    virtual ~ContextNode(void)          { }

    bool  readable(void)                { return _permission & CL_PERM_READ; }
    bool  writable(void)                { return _permission & CL_PERM_WRITE; }

    bool read(Datum& value)             { return this->r(value); }
    bool write(const Datum& value)      { return this->w(value); }

    virtual bool r(Datum& value)        = 0;
    virtual bool w(const Datum& value)  = 0;

  protected:
    unsigned short            _permission;
};

#endif // #ifndef SYMPHONY_CL_NODE_BASE
