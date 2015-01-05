/*
 * $Id: ast-object.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_OBJECT
#define H_SYMPHONY_MUSCL_AST_OBJECT

#include "ast.h"
#include <string>
#include <queue>

using std::queue;

class Datum;

class AstObject: public Ast {
  public:
    // constructors...
    AstObject(void);
    AstObject(const std::string& name);
    virtual ~AstObject(void);

    // public accessors...
    void                parent(AstObject* const parent)         { _parent = parent; }
    AstObject*          parent(void) const                      { return _parent; }
    void                child(AstObject* const child)           { _child = child; }
    AstObject*          child(void) const                       { return _child; }
    Datum*              name(void) const                        { return _name; }
    std::string         fullName(void) const;

    // general methods...
    void                addChild(AstObject* child);
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstObject*  _parent;
    AstObject*  _child;
    Datum*      _name;
};

#endif // H_SYMPHONY_MUSCL_AST_OBJECT
