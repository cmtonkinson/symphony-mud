/*
 * $Id: symbol.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_SYMBOL
#define H_SYMPHONY_MUSCL_SYMBOL

#include <list>
#include <string>

#define SYMBOL_ERROR  0
#define SYMBOL_NEW    1 // no semantic meaning yet
#define SYMBOL_CONST  2 // constant
#define SYMBOL_VAR    3 // variable
#define SYMBOL_FUNC   4 // function

class Ast;
class AstIdentifierList;
class Datum;

class Symbol {
  public:
    // Constructors...
    Symbol(void);
    Symbol(const std::string& name, unsigned type = SYMBOL_NEW, int line = -1);
    ~Symbol(void);

    // public accessors...
    void                type(const unsigned& type)          { _type = type; }
    unsigned            type(void) const                    { return _type; }
    void                name(const std::string& name)       { _name = name; }
    std::string         name(void) const                    { return _name; }
    void                symlist(AstIdentifierList* symlist) { _symlist = symlist; }
    AstIdentifierList*  symlist(void) const                 { return _symlist; }
    void                code(Ast* code)                     { _code = code; }
    Ast*                code(void) const                    { return _code; }
    void                offset(const int& offset)           { _offset = offset; }
    int                 offset(void) const                  { return _offset; }
    void                line(const int& line)               { _line = line; }
    int                 line(void) const                    { return _line; }

    // general methods...
    bool                isNew(void) const                   { return type() == SYMBOL_NEW; }
    bool                isConstant(void) const              { return type() == SYMBOL_CONST; }
    bool                isVariable(void) const              { return type() == SYMBOL_VAR; }
    bool                isFunction(void) const              { return type() == SYMBOL_FUNC; }
    const char*         stringType(void) const;
    std::string         dump(void) const;

  private:
    // data...
    unsigned            _type;
    std::string         _name;
    AstIdentifierList*  _symlist; // arg list (for functions)
    Ast*                _code;    // code (for functions)
    int                 _offset;  // address (used during code generation)
    int                 _line;    // line on which the symbol was declared
};

#endif // !H_SYMPHONY_MUSCL_SYMBOL
