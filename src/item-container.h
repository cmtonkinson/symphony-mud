
#ifndef H_SYMPHONY_ITEM_CONTAINER
#define H_SYMPHONY_ITEM_CONTAINER

#include "inventoryContainer.h"

class ItemContainer {
  public:
    // constructors...
    ItemContainer(void);
    ItemContainer(const ItemContainer& ref);
    ~ItemContainer(void);

    // accessor methods...
    InventoryContainer&       inventory(void)         { return _inventory; }
    const InventoryContainer& inventory(void) const   { return _inventory; }

  private:
    InventoryContainer        _inventory;
};

#endif // H_SYMPHONY_ITEM_CONTAINER
