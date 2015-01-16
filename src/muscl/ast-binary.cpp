
#include <stdio.h>
#include "ast-binary.h"
#include "ast-reference.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstBinaryOperation::AstBinaryOperation(void):
    _left(NULL),
    _right(NULL) {
  return;
}

AstBinaryOperation::AstBinaryOperation(const unsigned& type, Ast* left, Ast* right):
    Ast(type),
    _left(left),
    _right(right) {
  return;
}

AstBinaryOperation::~AstBinaryOperation(void) {
  return;
}

Datum* AstBinaryOperation::eval(void) {
  return NULL;
}

void AstBinaryOperation::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  left()->print(indent, false);
  right()->print(indent, true);
  return;
}

InstrList& AstBinaryOperation::codegen(void) {
  Record r;
  r.opcode(toOpcode());
  append(left()->codegen());
  append(right()->codegen());
  getArg(r.left(), left(), -(1+right()->instructions().size()));
  getArg(r.right(), right(), -1);
  append(r);
  return instructions();
}
