/*
 * $Id: ast-expr-list.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_EXPR_LIST
#define H_SYMPHONY_MUSCL_AST_EXPR_LIST

#include "ast.h"

class Datum;

class AstExpressionList: public Ast {
  public:
    // constructors...
    AstExpressionList(void);
    AstExpressionList(Ast* node, AstExpressionList* next);
    virtual ~AstExpressionList(void);

    // public accessors...
    void                node(Ast* node)               { _node = node; }
    Ast*                node(void) const              { return _node; }
    void                next(AstExpressionList* next) { _next = next; }
    AstExpressionList*  next(void) const              { return _next; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*                _node;
    AstExpressionList*  _next;
};

#endif // H_SYMPHONY_MUSCL_AST_EXPR_LIST
