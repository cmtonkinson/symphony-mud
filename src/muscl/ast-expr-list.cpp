
#include <stdio.h>
#include "ast-expr-list.h"
#include "datum.h"

AstExpressionList::AstExpressionList(void):
    _node(NULL),
    _next(NULL) {
  return;
}

AstExpressionList::AstExpressionList(Ast* node, AstExpressionList* next):
    Ast(AST_EXPR_LIST),
    _node(node),
    _next(next) {
  return;
}

AstExpressionList::~AstExpressionList(void) {
  return;
}

Datum* AstExpressionList::eval(void) {
  return NULL;
}

void AstExpressionList::print(std::string indent, bool last) {
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
  for (n = this; n; n = n->next()) {
    if (n->node()) {
      n->node()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstExpressionList::codegen(void) {
  return instructions();
}
