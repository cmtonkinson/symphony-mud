
#include <stdio.h>
#include "ast-unary.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstUnaryOperation::AstUnaryOperation(void):
    _node(NULL){
  return;
}

AstUnaryOperation::AstUnaryOperation(const unsigned& type, Ast* node):
    Ast(type),
    _node(node) {
  return;
}

AstUnaryOperation::~AstUnaryOperation(void) {
  return;
}

Datum* AstUnaryOperation::eval(void) {
  return NULL;
}

void AstUnaryOperation::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  node()->print(indent, true);
  return;
}

InstrList& AstUnaryOperation::codegen(void) {
  Record r;
  append(node()->codegen());
  r.opcode(toOpcode());
  getArg(r.left(), node(), -1);
  append(r);
  return instructions();
}
