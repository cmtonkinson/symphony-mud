
#ifndef H_SYMPHONY_OBJECT_CONTAINER
#define H_SYMPHONY_OBJECT_CONTAINER

#include "flagTable.h"
#include "inventoryContainer.h"
#include "mysql.h"

class ObjContainer {
  public:
    // constructors...
    ObjContainer(void);
    ObjContainer(const ObjContainer& ref);
    ObjContainer(ROW row);
    ~ObjContainer(void);

    // accessor methods...
    InventoryContainer&       inventory(void)         { return _inventory; }
    const InventoryContainer& inventory(void) const   { return _inventory; }
    FlagBank&                 flags(void)             { return _flags; }
    const FlagBank&           flags(void) const       { return _flags; }

  private:
    InventoryContainer        _inventory;
    FlagBank                  _flags;
};

#endif // H_SYMPHONY_OBJECT_CONTAINER
