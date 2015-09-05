
#include "item-container.hpp"

ItemContainer::ItemContainer(void) {
  return;
}

ItemContainer::ItemContainer(const ItemContainer& ref): _items(ref.items()) {
  return;
}

ItemContainer::~ItemContainer(void) {
  return;
}

void ItemContainer::add(Item* item) {
  items().push_back(item);
  return;
}

void ItemContainer::remove(Item* item) {
  items().remove(item);
  return;
}

bool ItemContainer::has(Item* item) const {
  for (auto iter : items()) {
    if (iter == item) return true;
  }
  return false;
}
