
#include <stdio.h>
#include "ast-assignment.h"
#include "ast-reference.h"
#include "symbol.h"
#include "record.h"
#include "vm.h"

AstAssignment::AstAssignment(void) {
  return;
}

AstAssignment::AstAssignment(unsigned type, AstReference* reference, Ast* value):
    Ast(type),
    _reference(reference),
    _value(value) {
  return;
}

AstAssignment::~AstAssignment(void) {
  return;
}

Datum* AstAssignment::eval(void) {
  return NULL;
}

void AstAssignment::print(std::string indent, bool last) {
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

InstrList& AstAssignment::codegen(void) {
  Record r;
  r.opcode(toOpcode());
  append(value()->codegen());
  r.left().symbol(reference()->symbol());
  getArg(r.right(), value(), -1);
  append(r);
  return instructions();
}
