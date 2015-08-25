
#include "modifier.hpp"

Modifier::Modifier(void):
    _attribute(0),
    _magnitude(0) {
  return;
}

Modifier::Modifier(const Modifier& ref):
    _attribute(ref.attribute()),
    _magnitude(ref.magnitude()) {
  return;
}

Modifier::Modifier(const unsigned short& attribute, const int& magnitude):
    _attribute(attribute),
    _magnitude(magnitude) {
  return;
}

Modifier::~Modifier(void) {
  return;
}

bool Modifier::operator== (const Modifier& ref) const {
  return ((_attribute == ref.attribute()) && (_magnitude == ref.magnitude()));
}
