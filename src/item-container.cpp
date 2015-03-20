
#include "item-container.h"

ItemContainer::ItemContainer(void): _inventory(&Identifiers::shortname) {
  return;
}

ItemContainer::ItemContainer(const ItemContainer& ref): _inventory(ref.inventory()) {
  return;
}

ItemContainer::~ItemContainer(void) {
  return;
}
