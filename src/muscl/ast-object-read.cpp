
#include <stdio.h>
#include "ast-item-read.h"
#include "ast-item.h"
#include "datum.h"
#include "record.h"
//#include "vm.h"

AstItemRead::AstItemRead(void) {
  return;
}

AstItemRead::AstItemRead(AstItem* item):
    Ast(AST_ITEM_READ),
    _item(item) {
  return;
}

AstItemRead::~AstItemRead(void) {
  return;
}

Datum* AstItemRead::eval(void) {
  return NULL;
}

void AstItemRead::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-%s (%s)\n", indent.c_str(), item()->fullName().c_str(), item()->stringType());
  return;
}

InstrList& AstItemRead::codegen(void) {
  Record r(toOpcode());
  r.left().value(item()->fullName());
  append(r);
  return instructions();
}
