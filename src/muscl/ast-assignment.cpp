/*
 * $Id: ast-assignment.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-assignment.h"
#include "ast-reference.h"
#include "symbol.h"
#include "record.h"
#include "vm.h"

AstAssignment::AstAssignment(void) {
  return;
}

AstAssignment::AstAssignment(unsigned type, AstReference* reference, Ast* value):
    Ast(type),
    _reference(reference),
    _value(value) {
  return;
}

AstAssignment::~AstAssignment(void) {
  return;
}

Datum* AstAssignment::eval(void) {
  return NULL;
}

void AstAssignment::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  reference()->print(indent, false);
  value()->print(indent, true);
  return;
}

InstrList& AstAssignment::codegen(void) {
  Record r;
  r.opcode(toOpcode());
  append(value()->codegen());
  r.left().symbol(reference()->symbol());
  getArg(r.right(), value(), -1);
  append(r);
  return instructions();
}
