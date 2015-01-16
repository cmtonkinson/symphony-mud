
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
