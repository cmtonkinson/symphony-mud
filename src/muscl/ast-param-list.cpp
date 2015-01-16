
#include <stdio.h>
#include "ast-param-list.h"
#include "ast-reference.h"
#include "datum.h"

AstParameterList::AstParameterList(void):
    _ref(NULL),
    _next(NULL) {
  return;
}

AstParameterList::AstParameterList(AstReference* ref, AstParameterList* next):
    Ast(AST_PARAM_LIST),
    _ref(ref),
    _next(next) {
  return;
}

AstParameterList::~AstParameterList(void) {
  return;
}

Datum* AstParameterList::eval(void) {
  return NULL;
}

void AstParameterList::print(std::string indent, bool last) {
  AstParameterList* n = NULL;
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
    if (n->ref()) {
      n->ref()->print(indent, !n->next());
    }
  }
  return;
}

InstrList& AstParameterList::codegen(void) {
  return instructions();
}
