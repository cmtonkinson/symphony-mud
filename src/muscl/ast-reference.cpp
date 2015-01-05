/*
 * $Id: ast-reference.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-reference.h"
#include "datum.h"
#include "symbol.h"

AstReference::AstReference(void) {
  return;
}

AstReference::AstReference(Symbol* symbol):
    Ast(AST_SYM_REF),
    _symbol(symbol) {
  return;
}

AstReference::~AstReference(void) {
  return;
}

Datum* AstReference::eval(void) {
  return NULL;
}

void AstReference::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-%s (%s)\n", indent.c_str(), symbol()->name().c_str(), symbol()->stringType());
  return;
}

InstrList& AstReference::codegen(void) {
  return instructions();
}
