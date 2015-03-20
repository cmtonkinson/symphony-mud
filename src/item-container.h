
#ifndef H_SYMPHONY_ITEM_CONTAINER
#define H_SYMPHONY_ITEM_CONTAINER

#include "inventoryContainer.h"

class ObjContainer {
  public:
    // constructors...
    ObjContainer(void);
    ObjContainer(const ObjContainer& ref);
    ~ObjContainer(void);

    // accessor methods...
    InventoryContainer&       inventory(void)         { return _inventory; }
    const InventoryContainer& inventory(void) const   { return _inventory; }

  private:
    InventoryContainer        _inventory;
};

#endif // H_SYMPHONY_ITEM_CONTAINER
