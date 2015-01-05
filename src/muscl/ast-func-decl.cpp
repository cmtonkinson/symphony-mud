/*
 * $Id: ast-func-decl.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "ast-func-decl.h"

#include <stdio.h>
#include "ast-param-list.h"
#include "ast-reference.h"
#include "symbol.h"
#include "symboltable.h"
#include "record.h"
#include "vm.h"

AstFunctionDeclaration::AstFunctionDeclaration(void) {
  return;
}

AstFunctionDeclaration::AstFunctionDeclaration(AstReference* reference, AstParameterList* parameters, Ast* body, SymbolTable* symtab):
    Ast(AST_FUNC_DECL),
    _reference(reference),
    _parameters(parameters),
    _body(body),
    _symtab(symtab) {
  return;
}

AstFunctionDeclaration::~AstFunctionDeclaration(void) {
  return;
}

Datum* AstFunctionDeclaration::eval(void) {
  return NULL;
}

void AstFunctionDeclaration::print(std::string indent, bool last) {
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
  parameters()->print(indent, false);
  body()->print(indent, true);
  return;
}

InstrList& AstFunctionDeclaration::codegen(void) {
  Record func, skip, parameter, ret;
  AstParameterList* n = NULL;
  unsigned num_params = 0;
  // opcode to mark the beginning of the function
  func.opcode(OPCODE_FUNC);
  getArg(func.left(), reference(), 0);
  append(func);
  // parameters
  parameter.opcode(OPCODE_PARAM);
  for (n = parameters(); n; n = n->next()) {
    if (n->ref()) {
      append(n->ref()->codegen());
      parameter.left().symbol(n->ref()->symbol());
      append(parameter);
      ++num_params;
    }
  }
  // function body
  append(body()->codegen());
  // append a return if one was not explicitly written
  if (instructions().back().opcode() != OPCODE_RETURN) {
    ret.opcode(OPCODE_RETURN);
    append(ret);
  }
  // branch around the function
  skip.opcode(OPCODE_BRA);
  skip.left().address(body()->instructions().size()+num_params+2);
  prepend(skip);
  return instructions();
}
