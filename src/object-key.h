
#ifndef H_SYMPHONY_OBJECT_KEY
#define H_SYMPHONY_OBJECT_KEY

#include "mysql.h"

class ObjKey {
  public:
    // Constructors...
    ObjKey(void);
    ObjKey(const ObjKey& ref);
    ObjKey(ROW row);
    ~ObjKey(void);
};

#endif // H_SYMPHONY_OBJECT_KEY
