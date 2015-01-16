
#include "ast-list.h"
#include <stdio.h>
#include "record.h"
#include "vm.h"
#include "ast-expr-list.h"
#include "datum.h"

AstList::AstList(void):
    _expressions(NULL) {
  return;
}

AstList::AstList(AstExpressionList* expressions):
    Ast(AST_LIST),
    _expressions(expressions) {
  return;
}

AstList::~AstList(void) {
  return;
}

Datum* AstList::eval(void) {
  return NULL;
}

void AstList::print(std::string indent, bool last) {
  AstExpressionList* n = NULL;
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  for (n = expressions(); n; n = n->next()) {
    if (n->node()) {
      n->node()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstList::codegen(void) {
  AstExpressionList* n = NULL;
  Record element;
  Record list;
  for (n = expressions(); n; n = n->next()) {
    if (n->node()) {
      append(n->node()->codegen());
      element.opcode(OPCODE_ELMT);
      getArg(element.left(), n->node(), -1);
      append(element);
    }
  }
  list.opcode(OPCODE_LIST);
  append(list);
  return instructions();
}
