/*
 * $Id: ast-value.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "ast-value.h"

#include <stdio.h>
#include "datum.h"
#include "record.h"
#include "vm.h"

AstValue::AstValue(void) {
  return;
}

AstValue::AstValue(Datum* value):
    Ast(AST_VALUE),
    _value(value) {
  return;
}

AstValue::~AstValue(void) {
  return;
}

Datum* AstValue::eval(void) {
  return NULL;
}

void AstValue::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-", indent.c_str());
  value()->print();
  printf("\n");
  return;
}

InstrList& AstValue::codegen(void) {
  Record r;
  r.opcode(OPCODE_VALUE);
  r.left().value(*value());
  append(r);
  return instructions();
}
