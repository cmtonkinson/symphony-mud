/*
 * $Id: ast-func-call.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "ast-func-call.h"

#include <stdio.h>
#include "ast-expr-list.h"
#include "ast-reference.h"
#include "symbol.h"
#include "record.h"
#include "vm.h"

AstFunctionCall::AstFunctionCall(void) {
  return;
}

AstFunctionCall::AstFunctionCall(AstReference* reference, AstExpressionList* arguments):
    Ast(AST_FUNC_CALL),
    _reference(reference),
    _arguments(arguments) {
  return;
}

AstFunctionCall::~AstFunctionCall(void) {
  return;
}

Datum* AstFunctionCall::eval(void) {
  return NULL;
}

void AstFunctionCall::print(std::string indent, bool last) {
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
  arguments()->print(indent, true);
  return;
}

InstrList& AstFunctionCall::codegen(void) {
  Record arg, call;
  AstExpressionList* n = NULL;
  // opcodes to assemble the arguments
  arg.opcode(OPCODE_ARG);
  for (n = arguments(); n; n = n->next()) {
    if (n->node()) {
      append(n->node()->codegen());
      getArg(arg.left(), n->node(), -1);
      append(arg);
    }
  }
  // opcode to invoke the function
  call.opcode(OPCODE_CALL);
  call.left().symbol(reference()->symbol());
  append(call);
  return instructions();
}
