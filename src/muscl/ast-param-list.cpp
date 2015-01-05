/*
 * $Id: ast-param-list.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-param-list.h"
#include "ast-reference.h"
#include "datum.h"

AstParameterList::AstParameterList(void):
    _ref(NULL),
    _next(NULL) {
  return;
}

AstParameterList::AstParameterList(AstReference* ref, AstParameterList* next):
    Ast(AST_PARAM_LIST),
    _ref(ref),
    _next(next) {
  return;
}

AstParameterList::~AstParameterList(void) {
  return;
}

Datum* AstParameterList::eval(void) {
  return NULL;
}

void AstParameterList::print(std::string indent, bool last) {
  AstParameterList* n = NULL;
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  for (n = this; n; n = n->next()) {
    if (n->ref()) {
      n->ref()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstParameterList::codegen(void) {
  return instructions();
}
