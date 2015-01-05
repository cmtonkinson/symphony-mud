/*
 * $Id: ast-assignment.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_ASSIGNMENT
#define H_SYMPHONY_MUSCL_AST_ASSIGNMENT

#include "ast.h"

class AstReference;

class AstAssignment: public Ast {
  public:
    // constructors...
    AstAssignment(void);
    AstAssignment(unsigned type, AstReference* reference, Ast* value);
    virtual ~AstAssignment(void);

    // public accessors...
    void          reference(AstReference* reference)  { _reference = reference; }
    AstReference* reference(void) const               { return _reference; }
    void          value(Ast* value)                   { _value = value; }
    Ast*          value(void) const                   { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference* _reference;
    Ast*          _value;
};

#endif // H_SYMPHONY_MUSCL_AST_ASSIGNMENT
