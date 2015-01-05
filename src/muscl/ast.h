/*
 * $Id: ast.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_MUSCL_AST
#define H_SYMPHONY_MUSCL_AST

#include <list>
#include <string>
#include <vector>

// random nodes
#define AST_ERROR        0
#define AST_VALUE        1
#define AST_CONST        2
#define AST_VAR          3
#define AST_SYM_REF      4
#define AST_LIST         5
#define AST_STMT_LIST    6
#define AST_EXPR_LIST    7
#define AST_SEND         8
#define AST_JUMP_CONT    9
#define AST_JUMP_BREAK  10

// object access
#define AST_OBJ         15
#define AST_OBJ_READ    16
#define AST_OBJ_WRITE   17

// function stuff
#define AST_PARAM_LIST  21
#define AST_FUNC_DECL   22
#define AST_FUNC_CALL   23
#define AST_FUNC_RETURN 24

// unary operations
#define AST_UN_MIN      30
#define AST_UN_NOT      31
#define AST_UN_CARD     32

// binary (mathematical) operations
#define AST_BIN_ADD     50
#define AST_BIN_SUB     51
#define AST_BIN_MUL     52
#define AST_BIN_DIV     53
#define AST_BIN_MOD     54
#define AST_BIN_POW     55

// binary (logical) operations
#define AST_BIN_AND     60
#define AST_BIN_OR      61

// binary (comparison) operations
#define AST_BIN_EQ     70
#define AST_BIN_NE     71
#define AST_BIN_LT     72
#define AST_BIN_LE     73
#define AST_BIN_GT     74
#define AST_BIN_GE     75

// assignment operations
#define AST_ASS         80
#define AST_ASS_ADD     81
#define AST_ASS_SUB     82
#define AST_ASS_MUL     83
#define AST_ASS_DIV     84
#define AST_ASS_MOD     85
#define AST_ASS_POW     86
#define AST_ASS_OBJ     87

// control structures
#define AST_CONT_IF       90
#define AST_CONT_IF_ELSE  91
#define AST_CONT_UNLESS   92
#define AST_CONT_FOR_TO   93
#define AST_CONT_FOR_IN   94
#define AST_CONT_WHILE    95
#define AST_CONT_DO_WHILE 96
#define AST_CONT_UNTIL    97
#define AST_CONT_DO_UNTIL 98

class Datum;
class Operand;
class Record;
class Symbol;

typedef std::list<Record> InstrList;
typedef std::vector<Record> CodeVect;

/* muscl Abstract Syntax Tree node */
class Ast { /* ADT */
  public:
    // constructors...
    Ast(void);
    Ast(const unsigned& type);
    virtual ~Ast(void);

    // public accessors...
    void              type(const unsigned& type)          { _type = type; }
    unsigned          type(void) const                    { return _type; }
    void              datatype(const unsigned& datatype)  { _datatype = datatype; }
    unsigned          datatype(void) const                { return _datatype; }
    InstrList&        instructions(void)                  { return _instructions; }
    const InstrList&  instructions(void) const            { return _instructions; }
    void              code(CodeVect* code)                { _code = code; }
    CodeVect&         code(void)                          { return *_code; }

    // general methods...
    const char*     stringType(void) const;
    int             toOpcode(void) const;
    virtual Datum*  eval(void) = 0;
    /* The algorithm used for the various Ast::print() implementations was taken Mar 2, 2010 from
     * http://stackoverflow.com/questions/1649027/how-do-i-print-out-a-tree-structure/1649223#1649223
     */
    virtual void        print(std::string indent, bool last) = 0;
    void                prepend(InstrList& more);
    void                prepend(Record& more);
    void                append(InstrList& more);
    void                append(Record& more);
    void                getArg(Operand& o, Ast* node, int offset);
    void                calculateOperand(Operand& o, int offset);
    virtual InstrList&  codegen(void);
    void                dumpcode(void);

  private:
    // data...
    unsigned    _type;
    unsigned    _datatype;
    InstrList   _instructions;
    CodeVect*   _code;
};

#endif // H_SYMPHONY_MUSCL_AST
