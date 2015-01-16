
#include <stdio.h>
#include "ast-jump.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstJump::AstJump(void):
    _expression(NULL) {
  return;
}

AstJump::AstJump(const unsigned& type, Ast* expression):
    Ast(type),
    _expression(expression) {
  return;
}

AstJump::~AstJump(void) {
  return;
}

Datum* AstJump::eval(void) {
  return NULL;
}

void AstJump::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  if (expression()) {
    expression()->print(indent, true);
  }
  return;
}

InstrList& AstJump::codegen(void) {
  Record r;
  r.opcode(toOpcode());
  if (expression()) {
    append(expression()->codegen());
    getArg(r.left(), expression(), -1);
  }
  append(r);
  return instructions();
}
