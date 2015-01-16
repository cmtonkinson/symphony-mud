
#include <stdio.h>
#include "ast-object-read.h"
#include "ast-object.h"
#include "datum.h"
#include "record.h"
//#include "vm.h"

AstObjectRead::AstObjectRead(void) {
  return;
}

AstObjectRead::AstObjectRead(AstObject* object):
    Ast(AST_OBJ_READ),
    _object(object) {
  return;
}

AstObjectRead::~AstObjectRead(void) {
  return;
}

Datum* AstObjectRead::eval(void) {
  return NULL;
}

void AstObjectRead::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-%s (%s)\n", indent.c_str(), object()->fullName().c_str(), object()->stringType());
  return;
}

InstrList& AstObjectRead::codegen(void) {
  Record r(toOpcode());
  r.left().value(object()->fullName());
  append(r);
  return instructions();
}
