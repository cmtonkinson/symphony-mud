
#include <stdio.h>
#include "ast-item-write.h"
#include "ast-item.h"
#include "record.h"
#include "vm.h"

AstItemWrite::AstItemWrite(void) {
  return;
}

AstItemWrite::AstItemWrite(AstItem* item, Ast* value):
    Ast(AST_ITEM_WRITE),
    _item(item),
    _value(value) {
  return;
}

AstItemWrite::~AstItemWrite(void) {
  return;
}

Datum* AstItemWrite::eval(void) {
  return NULL;
}

void AstItemWrite::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  item()->print(indent, false);
  value()->print(indent, true);
  return;
}

InstrList& AstItemWrite::codegen(void) {
  Record r(toOpcode());
  append(value()->codegen());
  r.left().value(item()->fullName());
  getArg(r.right(), value(), -1);
  append(r);
  return instructions();
}
