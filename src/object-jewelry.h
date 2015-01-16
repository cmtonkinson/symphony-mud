
#ifndef H_SYMPHONY_OBJECT_JEWELRY
#define H_SYMPHONY_OBJECT_JEWELRY

#include "mysql.h"

class ObjJewelry {
  public:
    // Constructors...
    ObjJewelry(void);
    ObjJewelry(const ObjJewelry& ref);
    ObjJewelry(ROW row);
    ~ObjJewelry(void);
};

#endif // H_SYMPHONY_OBJECT_JEWELRY
