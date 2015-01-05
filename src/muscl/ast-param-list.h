/*
 * $Id: ast-param-list.h 458 2010-08-02 19:34:56Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST_PARAM_LIST
#define H_SYMPHONY_MUSCL_AST_PARAM_LIST

#include "ast.h"

class AstReference;
class Datum;

class AstParameterList: public Ast {
  public:
    // constructors...
    AstParameterList( void );
    AstParameterList( AstReference* node, AstParameterList* next );
    virtual ~AstParameterList( void );

    // public accessors...
    void                ref( AstReference* ref )        { _ref = ref; }
    AstReference*       ref( void ) const               { return _ref; }
    void                next( AstParameterList* next )  { _next = next; }
    AstParameterList*   next( void ) const              { return _next; }

    // general methods...
    virtual Datum*      eval( void );
    virtual void        print( std::string indent, bool last );
    virtual InstrList&  codegen( void );

  private:
    // data...
    AstReference*       _ref;
    AstParameterList*   _next;
};

#endif // H_SYMPHONY_MUSCL_AST_PARAM_LIST
