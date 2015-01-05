/*
 * $Id: ast-func-decl.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_FUNC_DECL
#define H_SYMPHONY_MUSCL_AST_FUNC_DECL

#include "ast.h"

class AstParameterList;
class AstReference;
class SymbolTable;

class AstFunctionDeclaration: public Ast {
  public:
    // constructors...
    AstFunctionDeclaration(void);
    AstFunctionDeclaration(AstReference* reference, AstParameterList* arguments, Ast* body, SymbolTable* symtab);
    virtual ~AstFunctionDeclaration(void);

    // public accessors...
    void                reference(AstReference* reference)        { _reference = reference; }
    AstReference*       reference(void) const                     { return _reference; }
    void                parameters(AstParameterList* parameters) { _parameters = parameters; }
    AstParameterList*  parameters(void) const                    { return _parameters; }
    void                body(Ast* body)                           { _body = body; }
    Ast*                body(void) const                          { return _body; }
    void                symtab(SymbolTable* symtab)               { _symtab = symtab; }
    SymbolTable*        symtab(void)                              { return _symtab; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference*       _reference;
    AstParameterList*   _parameters;
    Ast*                _body;
    SymbolTable*        _symtab;
};

#endif // H_SYMPHONY_MUSCL_AST_FUNC_DECL
