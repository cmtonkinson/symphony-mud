
#include "operand.h"
#include <sstream>
#include <stdio.h>
#include "datum.h"
#include "symbol.h"

Operand::Operand(void):
    _address(0),
    _symbol(NULL) {
  return;
}

Operand::~Operand(void) {
  return;
}

bool Operand::empty(void) const {
  if (address() > 0 || value().data() || symbol()) return false;
  else return true;
}

void Operand::print(void) const {
  if (address() > 0) {
    printf(" @%-20d", address());
  } else if (symbol()) {
    printf(" *%-20s", symbol()->name().c_str());
  } else if (value().data()) {
    printf(" %-21s", value().serialize().c_str());
  } else {
    printf("                      ");
  }
  return;
}

string Operand::serialize(void) const {
  string dest;
  std::ostringstream buffer;
  if (address() > 0) buffer << address();
  else if (value().data()) buffer << value().serialize();
  return dest.append(buffer.str());
}

Operand Operand::unserialize(const char* serialized, int start) {
  Operand dest;
  return dest;
}
