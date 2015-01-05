/*
 * $Id: record.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "record.h"

#include "datum.h"
#include "symbol.h"
#include "vm.h"

Record::Record(void):
    _number(0),
    _opcode(0) {
  return;
}

Record::Record(const int& opcode):
    _number(0),
    _opcode(opcode) {
  return;
}

Record::~Record(void) {
  return;
}

void Record::print(void) const {
  printf(" |%4d | %-9s |", number(), opcodeToString());
  left().print();
  printf(" |");
  right().print();
  printf(" |\n");
  return;
}

const char* Record::opcodeToString(void) const {
  switch (opcode()) {
    // miscellaneous
    case OPCODE_HALT:     return "HALT";
    case OPCODE_SEND:     return "SEND";
    // data
    case OPCODE_VALUE:    return "VALUE";
    case OPCODE_CONST:    return "CONST";
    case OPCODE_VAR:      return "VAR";
    case OPCODE_STORE:    return "STORE";
    // objecs
    case OPCODE_OBJ_NAME:   return "OBJ_NAME";
    case OPCODE_OBJ_INIT:   return "OBJ_INIT";
    case OPCODE_OBJ_READ:   return "OBJ_READ";
    case OPCODE_OBJ_WRITE:  return "OBJ_WRITE";
    // lists
    case OPCODE_ELMT:     return "ELMT";
    case OPCODE_LIST:     return "LIST";
    case OPCODE_ITER_NEW: return "ITER_NEW";
    case OPCODE_ITER_INC: return "ITER_INC";
    case OPCODE_ITER_END: return "ITER_END";
    // branching
    case OPCODE_BRA:    return "BRA";
    case OPCODE_BRT:    return "BRT";
    case OPCODE_BRF:    return "BRF";
    // functions
    case OPCODE_FUNC:   return "FUNC";
    case OPCODE_PARAM:  return "PARAM";
    case OPCODE_ARG:    return "ARG";
    case OPCODE_CALL:   return "CALL";
    case OPCODE_RETURN: return "RETRN";
    // logical operations
    case OPCODE_AND:    return "AND";
    case OPCODE_OR:     return "OR";
    case OPCODE_NOT:    return "NOT";
    // comparisons
    case OPCODE_COMP:   return "COMP";
    case OPCODE_EQ:     return "EQ";
    case OPCODE_NE:     return "NE";
    case OPCODE_LT:     return "LT";
    case OPCODE_LE:     return "LE";
    case OPCODE_GT:     return "GT";
    case OPCODE_GE:     return "GE";
    // mathematical operations
    case OPCODE_ADD:    return "ADD";
    case OPCODE_SUB:    return "SUB";
    case OPCODE_MUL:    return "MUL";
    case OPCODE_DIV:    return "DIV";
    case OPCODE_MOD:    return "MOD";
    case OPCODE_EXP:    return "EXP";
    case OPCODE_MIN:    return "MIN";
    case OPCODE_CARD:   return "CARD";
    // complex storage operations
    case OPCODE_ADDSTORE: return "ADDSTORE";
    case OPCODE_SUBSTORE: return "SUBSTORE";
    case OPCODE_MULSTORE: return "MULSTORE";
    case OPCODE_DIVSTORE: return "DIVSTORE";
    case OPCODE_MODSTORE: return "MODSTORE";
    case OPCODE_POWSTORE: return "POWSTORE";
    // error
    default:  return "ERR";
  }
}
