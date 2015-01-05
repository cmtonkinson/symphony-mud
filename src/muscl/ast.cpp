/*
 * $Id: ast.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include <stdio.h>
#include "ast.h"
#include "ast-reference.h"
#include "symbol.h"
#include "operand.h"
#include "record.h"
#include "vm.h"

Ast::Ast(void):
    _type(AST_ERROR),
    _datatype(0),
    _code(NULL) {
  return;
}

Ast::Ast(const unsigned& type): _type(type) {
  return;
}

Ast::~Ast(void) {
  return;
}

const char* Ast::stringType(void) const {
  switch (type()) {
    case AST_VALUE:       return "value";
    case AST_CONST:       return "declaration (constant)";
    case AST_VAR:         return "declaration (variable)";
    case AST_SYM_REF:     return "symbol reference";
    case AST_LIST:        return "list";
    case AST_STMT_LIST:   return "statement list";
    case AST_EXPR_LIST:   return "expression list";
    case AST_SEND:        return "send statement";
    case AST_JUMP_CONT:   return "continue";
    case AST_JUMP_BREAK:  return "break";

    case AST_OBJ:       return "object reference";
    case AST_OBJ_READ:  return "object read";
    case AST_OBJ_WRITE: return "object write";

    case AST_PARAM_LIST:  return "parameter list";
    case AST_FUNC_DECL:   return "function declaration";
    case AST_FUNC_CALL:   return "function call";
    case AST_FUNC_RETURN: return "function return";

    case AST_UN_MIN:    return "unary expression (minus)";
    case AST_UN_NOT:    return "unary expression (logical not)";
    case AST_UN_CARD:   return "unary expression (cardinality)";

    case AST_BIN_ADD:   return "binary op (addition)";
    case AST_BIN_SUB:   return "binary op (subtraction)";
    case AST_BIN_MUL:   return "binary op (multiplication)";
    case AST_BIN_DIV:   return "binary op (division)";
    case AST_BIN_MOD:   return "binary op (mod)";
    case AST_BIN_POW:   return "binary op (exponent)";

    case AST_BIN_AND:   return "binary op (logical and)";
    case AST_BIN_OR:    return "binary op (logical or)";

    case AST_BIN_EQ:   return "binary op (==)";
    case AST_BIN_NE:   return "binary op (!=)";
    case AST_BIN_LT:   return "binary op (<)";
    case AST_BIN_LE:   return "binary op (<=)";
    case AST_BIN_GT:   return "binary op (>)";
    case AST_BIN_GE:   return "binary op (>=)";

    case AST_ASS:       return "assignment";
    case AST_ASS_ADD:   return "assignment (w/addition)";
    case AST_ASS_SUB:   return "assignment (w/subtraction)";
    case AST_ASS_MUL:   return "assignment (w/multiplication)";
    case AST_ASS_DIV:   return "assignment (w/division)";
    case AST_ASS_MOD:   return "assignment (w/mod)";
    case AST_ASS_POW:   return "assignment (w/pow)";
    case AST_ASS_OBJ:   return "assignment (to object)";

    case AST_CONT_IF:       return "conditional (if)";
    case AST_CONT_IF_ELSE:  return "conditional (if-else)";
    case AST_CONT_UNLESS:   return "conditional (unless)";
    case AST_CONT_FOR_TO:   return "loop (for-to)";
    case AST_CONT_FOR_IN:   return "loop (for-in)";
    case AST_CONT_WHILE:    return "loop (while)";
    case AST_CONT_DO_WHILE: return "loop (do-while)";
    case AST_CONT_UNTIL:    return "loop (until)";
    case AST_CONT_DO_UNTIL: return "loop (do-until)";

    default:  return "undefined";
  }
}

int Ast::toOpcode(void) const {
  switch (type()) {
    case AST_FUNC_CALL:   return OPCODE_CALL;
    case AST_FUNC_RETURN: return OPCODE_RETURN;

    case AST_VALUE:       return OPCODE_VALUE;
    case AST_CONST:       return OPCODE_CONST;
    case AST_VAR:         return OPCODE_VAR;

    case AST_OBJ_READ:  return OPCODE_OBJ_READ;
    case AST_OBJ_WRITE: return OPCODE_OBJ_WRITE;

    case AST_ASS:       return OPCODE_STORE;
    case AST_ASS_ADD:   return OPCODE_ADDSTORE;
    case AST_ASS_SUB:   return OPCODE_SUBSTORE;
    case AST_ASS_MUL:   return OPCODE_MULSTORE;
    case AST_ASS_DIV:   return OPCODE_DIVSTORE;
    case AST_ASS_MOD:   return OPCODE_MODSTORE;
    case AST_ASS_POW:   return OPCODE_POWSTORE;

    case AST_UN_MIN:      return OPCODE_MIN;
    case AST_UN_NOT:      return OPCODE_NOT;
    case AST_UN_CARD:     return OPCODE_CARD;

    case AST_BIN_ADD:     return OPCODE_ADD;
    case AST_BIN_SUB:     return OPCODE_SUB;
    case AST_BIN_MUL:     return OPCODE_MUL;
    case AST_BIN_DIV:     return OPCODE_DIV;
    case AST_BIN_MOD:     return OPCODE_MOD;
    case AST_BIN_POW:     return OPCODE_EXP;

    case AST_BIN_AND:     return OPCODE_AND;
    case AST_BIN_OR:      return OPCODE_OR;

    case AST_BIN_EQ:      return OPCODE_EQ;
    case AST_BIN_NE:      return OPCODE_NE;
    case AST_BIN_LT:      return OPCODE_LT;
    case AST_BIN_LE:      return OPCODE_LE;
    case AST_BIN_GT:      return OPCODE_GT;
    case AST_BIN_GE:      return OPCODE_GE;

    default:              return OPCODE_ERR;
  }
}

void Ast::prepend(InstrList& more) {
  instructions().insert(instructions().begin(), more.begin(), more.end());
  return;
}

void Ast::prepend(Record& more) {
  instructions().push_front(more);
  return;
}

void Ast::append(InstrList& more) {
  instructions().insert(instructions().end(), more.begin(), more.end());
  return;
}

void Ast::append(Record& more) {
  instructions().push_back(more);
  return;
}

void Ast::getArg(Operand& o, Ast* node, int offset) {
  if (node->type() == AST_SYM_REF) {
    o.symbol(((AstReference*)node)->symbol());
    o.address(0);
  } else {
    o.symbol(NULL);
    o.address(offset);
  }
  o.value(NULL);
  return;
}

void Ast::calculateOperand(Operand& o, int offset) {
  /*
   * This method supplies fundamental backpatching logic.
   *
   * If the operand is a symbol reference, then we need to either
   * initialize the address for dynamic storage (in the case that
   * this is the first time we've seen this symbol), or retrieve
   * the address of the symbols' first entry (for all references
   * thereafter).
   * Otherwise, the address given is a relative offset, so we add
   * that to the "current" instruction address to compute the
   * appropriate absolute offset for the operand in question.
   */
  int address = o.address();
  if (o.symbol()) {
    if (o.symbol()->offset() == -1) {
      o.symbol()->offset(offset);
      o.address(offset);
    } else {
      o.address(o.symbol()->offset());
    }
  } else {
    address += address == 0 ? 0 : offset;
    o.address(address);
  }
  return;
}

InstrList& Ast::codegen(void) {
  return instructions();
}

void Ast::dumpcode(void) {
  Record halt(OPCODE_HALT);
  unsigned op_num = 0;
  // append the halt instruction
  append(halt);
  printf(" +------+----------+-----------------------+-----------------------+\n");
  for (std::list<Record>::iterator it = instructions().begin(); it != instructions().end(); ++it) {
    // first, assign the proper record number
    it->number(++op_num);
    // then, either calculate the symbol offset, or backpatch
    calculateOperand(it->left(), it->number());
    calculateOperand(it->right(), it->number());
    // last, print the record
    it->print();
  }
  printf(" +------+----------+-----------------------+-----------------------+\n");
  code(new CodeVect());
  code().push_back(halt);
  code().insert(code().end(), instructions().begin(), instructions().end());
  return;
}
