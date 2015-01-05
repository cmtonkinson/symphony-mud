/*
 * $Id: ast-object-read.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_OBJ_READ
#define H_SYMPHONY_MUSCL_AST_OBJ_READ

#include "ast.h"

class Datum;
class AstObject;

class AstObjectRead: public Ast {
  public:
    // constructors...
    AstObjectRead(void);
    AstObjectRead(AstObject* object);
    virtual ~AstObjectRead(void);

    // public accessors...
    void                object(AstObject* object)             { _object = object; }
    AstObject*          object(void) const                    { return _object; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstObject*          _object;
};

#endif // H_SYMPHONY_MUSCL_AST_OBJ_READ
