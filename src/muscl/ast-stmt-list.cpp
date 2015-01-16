
#include <stdio.h>
#include "ast-stmt-list.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstStatementList::AstStatementList(void):
    _node(NULL),
    _next(NULL) {
  return;
}

AstStatementList::AstStatementList(Ast* node, AstStatementList* next):
    Ast(AST_STMT_LIST),
    _node(node),
    _next(next) {
  return;
}

AstStatementList::~AstStatementList(void) {
  return;
}

Datum* AstStatementList::eval(void) {
  return NULL;
}

void AstStatementList::print(std::string indent, bool last) {
  AstStatementList* n = NULL;
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

InstrList& AstStatementList::codegen(void) {
  AstStatementList* n = NULL;
  for (n = this; n; n = n->next()) {
    if (n->node()) {
      n->node()->codegen();
      append(n->node()->instructions());
    }
  }
  return instructions();
}
