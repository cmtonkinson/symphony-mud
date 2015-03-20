
#include <stdio.h>
#include "ast-item.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstItem::AstItem(void):
    _parent(NULL),
    _child(NULL),
    _name(NULL) {
  return;
}

AstItem::AstItem(const std::string& name):
    Ast(AST_ITEM),
    _parent(NULL),
    _child(NULL),
    _name(new Datum(name)) {
  return;
}

AstItem::~AstItem(void) {
  return;
}

std::string AstItem::fullName(void) const {
  std::string n;
  AstItem const* o = this;
  do {
    n.append(1, ':').append(o->name()->toString());
    o = o->child();
  } while (o);
  return n;
}

void AstItem::addChild(AstItem* child) {
  this->child(child);
  child->parent(this);
  return;
}

Datum* AstItem::eval(void) {
  return NULL;
}

void AstItem::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  printf("%s\\-%s (%s)\n", indent.c_str(), fullName().c_str(), stringType());
  return;
}

InstrList& AstItem::codegen(void) {
  return instructions();
}
