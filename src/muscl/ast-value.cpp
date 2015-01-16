
#include "ast-value.h"
#include <stdio.h>
#include "datum.h"
#include "record.h"
#include "vm.h"

AstValue::AstValue(void) {
  return;
}

AstValue::AstValue(Datum* value):
    Ast(AST_VALUE),
    _value(value) {
  return;
}

AstValue::~AstValue(void) {
  return;
}

Datum* AstValue::eval(void) {
  return NULL;
}

void AstValue::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-", indent.c_str());
  value()->print();
  printf("\n");
  return;
}

InstrList& AstValue::codegen(void) {
  Record r;
  r.opcode(OPCODE_VALUE);
  r.left().value(*value());
  append(r);
  return instructions();
}
