
#include <stdio.h>
#include "ast-object.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstObject::AstObject(void):
    _parent(NULL),
    _child(NULL),
    _name(NULL) {
  return;
}

AstObject::AstObject(const std::string& name):
    Ast(AST_OBJ),
    _parent(NULL),
    _child(NULL),
    _name(new Datum(name)) {
  return;
}

AstObject::~AstObject(void) {
  return;
}

std::string AstObject::fullName(void) const {
  std::string n;
  AstObject const* o = this;
  do {
    n.append(1, ':').append(o->name()->toString());
    o = o->child();
  } while (o);
  return n;
}

void AstObject::addChild(AstObject* child) {
  this->child(child);
  child->parent(this);
  return;
}

Datum* AstObject::eval(void) {
  return NULL;
}

void AstObject::print(std::string indent, bool last) {
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

InstrList& AstObject::codegen(void) {
  return instructions();
}
