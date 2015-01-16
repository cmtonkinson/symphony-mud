
#ifndef H_SYMPHONY_OBJECT_CLOTHING
#define H_SYMPHONY_OBJECT_CLOTHING

#include "mysql.h"

class ObjClothing {
  public:
    // Constructors...
    ObjClothing(void);
    ObjClothing(const ObjClothing& ref);
    ObjClothing(ROW row);
    ~ObjClothing(void);
};

#endif // H_SYMPHONY_OBJECT_CLOTHING
