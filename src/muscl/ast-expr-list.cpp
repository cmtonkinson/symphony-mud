/*
 * $Id: ast-expr-list.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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
#include "ast-expr-list.h"
#include "datum.h"

AstExpressionList::AstExpressionList(void):
    _node(NULL),
    _next(NULL) {
  return;
}

AstExpressionList::AstExpressionList(Ast* node, AstExpressionList* next):
    Ast(AST_EXPR_LIST),
    _node(node),
    _next(next) {
  return;
}

AstExpressionList::~AstExpressionList(void) {
  return;
}

Datum* AstExpressionList::eval(void) {
  return NULL;
}

void AstExpressionList::print(std::string indent, bool last) {
  AstExpressionList* n = NULL;
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
    if (n->node()) {
      n->node()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstExpressionList::codegen(void) {
  return instructions();
}
