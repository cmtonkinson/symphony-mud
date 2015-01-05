/*
 * $Id: ast-binary.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_BINARY
#define H_SYMPHONY_MUSCL_AST_BINARY

#include "ast.h"

class Datum;

class AstBinaryOperation: public Ast {
  public:
    // constructors...
    AstBinaryOperation(void);
    AstBinaryOperation(const unsigned& type, Ast* left, Ast* right);
    virtual ~AstBinaryOperation(void);

    // public accessors...
    void  left(Ast* left)     { _left = left; }
    Ast*  left(void) const    { return _left; }
    void  right(Ast* right)   { _right = right; }
    Ast*  right(void) const   { return _right; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*  _left;
    Ast*  _right;
};

#endif // H_SYMPHONY_MUSCL_AST_BINARY
