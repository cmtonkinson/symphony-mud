
#include "symbol.h"
#include "symboltable.h"

SymbolTable::SymbolTable(void) {
  return;
}

SymbolTable::SymbolTable(const std::string& name):
    _name(name) {
  return;
}

SymbolTable::~SymbolTable(void) {
  for (std::map<std::string,Symbol*>::iterator it = table().begin(); it != table().end(); ++it) delete it->second;
  return;
}

void SymbolTable::insert(Symbol* symbol) { // public
  table()[symbol->name()] = symbol;
  return;
}

void SymbolTable::remove(Symbol* symbol) { // public
  table().erase(symbol->name());
  return;
}

Symbol* SymbolTable::lookup(const std::string& name) { // public
  std::map<std::string,Symbol*>::iterator it;
  if ((it = table().find(name)) == table().end()) return NULL;
  else return it->second;
}

std::string SymbolTable::dump(void) const {
  std::string buf;
  for (std::map<std::string,Symbol*>::const_iterator it = table().begin(); it != table().end(); ++it) {
    buf.append("  ").append(it->second->dump()).append(1, '\n');
  }
  return buf;
}
