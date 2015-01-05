/*
 * $Id: ast-list.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "ast-list.h"

#include <stdio.h>
#include "record.h"
#include "vm.h"
#include "ast-expr-list.h"
#include "datum.h"

AstList::AstList(void):
    _expressions(NULL) {
  return;
}

AstList::AstList(AstExpressionList* expressions):
    Ast(AST_LIST),
    _expressions(expressions) {
  return;
}

AstList::~AstList(void) {
  return;
}

Datum* AstList::eval(void) {
  return NULL;
}

void AstList::print(std::string indent, bool last) {
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
  for (n = expressions(); n; n = n->next()) {
    if (n->node()) {
      n->node()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstList::codegen(void) {
  AstExpressionList* n = NULL;
  Record element;
  Record list;
  for (n = expressions(); n; n = n->next()) {
    if (n->node()) {
      append(n->node()->codegen());
      element.opcode(OPCODE_ELMT);
      getArg(element.left(), n->node(), -1);
      append(element);
    }
  }
  list.opcode(OPCODE_LIST);
  append(list);
  return instructions();
}
