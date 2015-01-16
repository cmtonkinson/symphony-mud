
#include "datum.h"
#include "scope.h"
#include "symbol.h"
#include "symboltable.h"

Scope::Scope(void) {
  return;
}

Scope::~Scope(void) {
  while (!frames().empty()) {
    delete frames().front();
    frames().erase(frames().begin());
  }
  return;
}

void Scope::check(void) {
  if (frames().size() < 2) throw std::string("Scope::check() failed! MUSCL runtime is borked. Settle in and have fun debugging!");
  return;
}

void Scope::pushFrame(const std::string& name) {
  frames().push_front(new SymbolTable(name));
  return;
}

SymbolTable* Scope::popFrame(void) {
  SymbolTable* top = NULL;
  check();
  top = frames().front();
  frames().pop_front();
  return top;
}

void Scope::insert(Symbol* symbol) {
  check();
  frames().front()->insert(symbol);
  return;
}

void Scope::remove(Symbol* symbol) {
  check();
  frames().front()->remove(symbol);
  return;
}

Symbol* Scope::lookup(const std::string& name) {
  std::list<SymbolTable*>::iterator it;
  Symbol* symbol = NULL;
  std::string effective_name(name.substr(0, 128));
  check();
  /* first, check the "top of stack" (the innermost symbol table) */
  if ((symbol = frames().front()->lookup(effective_name)) != NULL) return symbol;
  /* if that failed, check the built-in scope */
  if ((symbol = frames().back()->lookup(effective_name)) != NULL) return symbol;
  /* if that failed, bail with a NULL */
  return NULL;
}

std::string Scope::dump(void) const {
  std::string buf("--==Symbol Table==--\n");
  for (std::list<SymbolTable*>::const_reverse_iterator it = frames().rbegin(); it != frames().rend(); ++it) {
    buf.append((*it)->name()).append(":\n").append((*it)->dump()).append("--------------------\n");
  }
  return buf;
}
