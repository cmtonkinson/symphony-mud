
#include "object-container.h"

ObjContainer::ObjContainer(void):
    _inventory(&Identifiers::shortname) {
  return;
}

ObjContainer::ObjContainer(const ObjContainer& ref):
    _inventory(ref.inventory()),
    _flags(ref.flags()) {
  return;
}

ObjContainer::ObjContainer(ROW row):
    _inventory(&Identifiers::shortname) {
  return;
}

ObjContainer::~ObjContainer(void) {
  return;
}
