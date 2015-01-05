/*
 * $Id: ast-conditional.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-conditional.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstConditional::AstConditional(void) {
  return;
}

AstConditional::AstConditional(const unsigned& type, Ast* condition, Ast* bodycode, Ast* elsecode):
    Ast(type),
    _condition(condition),
    _bodycode(bodycode),
    _elsecode(elsecode) {
  return;
}

AstConditional::~AstConditional(void) {
  return;
}

Datum* AstConditional::eval(void) {
  return NULL;
}

void AstConditional::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  condition()->print(indent, !bodycode() && !elsecode());
  if (bodycode()) {
    bodycode()->print(indent, !elsecode());
  }
  if (elsecode()) {
    elsecode()->print(indent, true);
  }
  return;
}

InstrList& AstConditional::codegen(void) {
  Record branch_out;  // leave/skip
  Record branch_in;   // enter/re-enter
  Record compare;     // test
  InstrList& body = bodycode()->codegen();
  InstrList& other = elsecode() ? elsecode()->codegen() : body; // body is just a placeholder here, it doesn't get used

  switch (type()) {
    case AST_CONT_IF:
      // test
      append(condition()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), condition(), -1);
      append(compare);
      // branch if test was false
      branch_out.opcode(OPCODE_BRF);
      branch_out.left().address(body.size()+1);
      append(branch_out);
      // body
      append(body);
      break;
    case AST_CONT_IF_ELSE:
      // test
      append(condition()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), condition(), -1);
      append(compare);
      // branch to the elsecode if test was false
      branch_out.opcode(OPCODE_BRF);
      branch_out.left().address(body.size()+2);
      append(branch_out);
      // body
      append(body);
      // branch around the elsecode
      branch_out.opcode(OPCODE_BRA);
      branch_out.left().address(other.size() + 1);
      append(branch_out);
      // tack on the elsecode
      append(other);
      break;
    case AST_CONT_UNLESS:
      // test
      append(condition()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), condition(), -1);
      append(compare);
      // branch if test was false
      branch_out.opcode(OPCODE_BRT);
      branch_out.left().address(body.size()+1);
      append(branch_out);
      // body
      append(body);
      break;
  }

  return instructions();
}
