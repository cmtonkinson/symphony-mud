
#include "datum.h"
#include "symbol.h"

Symbol::Symbol(void):
    _type(SYMBOL_NEW),
    _synlist(NULL),
    _code(NULL),
    _offset(-1),
    _line(-1) {
  return;
}

Symbol::Symbol(const std::string& name, unsigned type, int line):
    _type(type),
    _name(name),
    _synlist(NULL),
    _code(NULL),
    _offset(-1),
    _line(line) {
  return;
}

Symbol::~Symbol(void) {
  return;
}

const char* Symbol::stringType(void) const {
  switch (type()) {
    case SYMBOL_NEW:    return "new";
    case SYMBOL_CONST:  return "constant";
    case SYMBOL_VAR:    return "variable";
    case SYMBOL_FUNC:   return "function";
    default:            return "undefined";
  }
}

std::string Symbol::dump(void) const {
  std::string buf("'");
  buf.append(name()).append("' (").append(stringType()).append(")");
  return buf;
}
