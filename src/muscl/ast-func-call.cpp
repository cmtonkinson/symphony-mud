
#include "ast-func-call.h"
#include <stdio.h>
#include "ast-expr-list.h"
#include "ast-reference.h"
#include "symbol.h"
#include "record.h"
#include "vm.h"

AstFunctionCall::AstFunctionCall(void) {
  return;
}

AstFunctionCall::AstFunctionCall(AstReference* reference, AstExpressionList* arguments):
    Ast(AST_FUNC_CALL),
    _reference(reference),
    _arguments(arguments) {
  return;
}

AstFunctionCall::~AstFunctionCall(void) {
  return;
}

Datum* AstFunctionCall::eval(void) {
  return NULL;
}

void AstFunctionCall::print(std::string indent, bool last) {
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
  arguments()->print(indent, true);
  return;
}

InstrList& AstFunctionCall::codegen(void) {
  Record arg, call;
  AstExpressionList* n = NULL;
  // opcodes to assemble the arguments
  arg.opcode(OPCODE_ARG);
  for (n = arguments(); n; n = n->next()) {
    if (n->node()) {
      append(n->node()->codegen());
      getArg(arg.left(), n->node(), -1);
      append(arg);
    }
  }
  // opcode to invoke the function
  call.opcode(OPCODE_CALL);
  call.left().symbol(reference()->symbol());
  append(call);
  return instructions();
}
