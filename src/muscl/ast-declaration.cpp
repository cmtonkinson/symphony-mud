
#include <stdio.h>
#include "ast-declaration.h"
#include "ast-reference.h"
#include "symbol.h"
#include "record.h"
#include "vm.h"

AstDeclaration::AstDeclaration(void) {
  return;
}

AstDeclaration::AstDeclaration(unsigned type, AstReference* reference, Ast* value):
    Ast(type),
    _reference(reference),
    _value(value) {
  return;
}

AstDeclaration::~AstDeclaration(void) {
  return;
}

Datum* AstDeclaration::eval(void) {
  return NULL;
}

void AstDeclaration::print(std::string indent, bool last) {
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

InstrList& AstDeclaration::codegen(void) {
  Record r;
  r.opcode(reference()->symbol()->isVariable() ? OPCODE_VAR : OPCODE_CONST);
  r.left().symbol(reference()->symbol());
  append(value()->codegen());
  getArg(r.right(), value(), -1);
  append(r);
  return instructions();
}
