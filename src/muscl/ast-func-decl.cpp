
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
