/*
 * $Id: ast-binary.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-binary.h"
#include "ast-reference.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstBinaryOperation::AstBinaryOperation(void):
    _left(NULL),
    _right(NULL) {
  return;
}

AstBinaryOperation::AstBinaryOperation(const unsigned& type, Ast* left, Ast* right):
    Ast(type),
    _left(left),
    _right(right) {
  return;
}

AstBinaryOperation::~AstBinaryOperation(void) {
  return;
}

Datum* AstBinaryOperation::eval(void) {
  return NULL;
}

void AstBinaryOperation::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  left()->print(indent, false);
  right()->print(indent, true);
  return;
}

InstrList& AstBinaryOperation::codegen(void) {
  Record r;
  r.opcode(toOpcode());
  append(left()->codegen());
  append(right()->codegen());
  getArg(r.left(), left(), -(1+right()->instructions().size()));
  getArg(r.right(), right(), -1);
  append(r);
  return instructions();
}
