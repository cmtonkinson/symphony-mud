
#ifndef H_SYMPHONY_OBJECT_FOOD
#define H_SYMPHONY_OBJECT_FOOD

#include "mysql.h"

class ObjFood {
  public:
    // Constructors...
    ObjFood(void);
    ObjFood(const ObjFood& ref);
    ObjFood(ROW row);
    ~ObjFood(void);
};

#endif // H_SYMPHONY_OBJECT_FOOD
