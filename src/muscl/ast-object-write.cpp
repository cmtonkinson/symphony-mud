
#include <stdio.h>
#include "ast-object-write.h"
#include "ast-object.h"
#include "record.h"
#include "vm.h"

AstObjectWrite::AstObjectWrite(void) {
  return;
}

AstObjectWrite::AstObjectWrite(AstObject* object, Ast* value):
    Ast(AST_OBJ_WRITE),
    _object(object),
    _value(value) {
  return;
}

AstObjectWrite::~AstObjectWrite(void) {
  return;
}

Datum* AstObjectWrite::eval(void) {
  return NULL;
}

void AstObjectWrite::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  object()->print(indent, false);
  value()->print(indent, true);
  return;
}

InstrList& AstObjectWrite::codegen(void) {
  Record r(toOpcode());
  append(value()->codegen());
  r.left().value(object()->fullName());
  getArg(r.right(), value(), -1);
  append(r);
  return instructions();
}
