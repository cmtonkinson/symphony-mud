
#include <stdio.h>
#include "ast-send.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstSend::AstSend(void):
    _node(NULL) {
  return;
}

AstSend::AstSend(Ast* node):
    Ast(AST_SEND),
    _node(node) {
  return;
}

AstSend::~AstSend(void) {
  return;
}

Datum* AstSend::eval(void) {
  return NULL;
}

void AstSend::print(std::string indent, bool last) {
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

InstrList& AstSend::codegen(void) {
  Record r;
  append(node()->codegen());
  r.opcode(OPCODE_SEND);
  getArg(r.left(), node(), -1);
  append(r);
  return instructions();
}
