
#ifndef H_SYMPHONY_OBJECT_ARMOR
#define H_SYMPHONY_OBJECT_ARMOR

#include "mysql.h"

class ObjArmor {
  public:
    // Constructors...
    ObjArmor(void);
    ObjArmor(const ObjArmor& ref);
    ObjArmor(ROW row);
    ~ObjArmor(void);
};

#endif // H_SYMPHONY_OBJECT_ARMOR
