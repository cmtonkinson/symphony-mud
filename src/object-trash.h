
#ifndef H_SYMPHONY_OBJECT_TRASH
#define H_SYMPHONY_OBJECT_TRASH

#include "mysql.h"

class ObjTrash {
  public:
    // Constructors...
    ObjTrash(void);
    ObjTrash(const ObjTrash& ref);
    ObjTrash(ROW row);
    ~ObjTrash(void);
};

#endif // H_SYMPHONY_OBJECT_TRASH
