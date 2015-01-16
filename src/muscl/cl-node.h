
#ifndef SYMPHONY_CL_NODE
#define SYMPHONY_CL_NODE

#include <string>
#include "datum.h"

#define CL_PERM_READ  4
#define CL_PERM_WRITE 2

using std::string;

class ContextNode {
  public:
    ContextNode(const unsigned short& permission):
        _permission(permission) {
      return;
    }
    virtual ~ContextNode(void)          { }

    bool  readable(void)                { return _permission & CL_PERM_READ; }
    bool  writable(void)                { return _permission & CL_PERM_WRITE; }

    bool read(Datum& value)             { return this->r(value); }
    bool write(const Datum& value)      { return this->w(value); }

    virtual bool r(Datum& value)        = 0;
    virtual bool w(const Datum& value)  = 0;

  protected:
    unsigned short            _permission;
};

#endif // #ifndef SYMPHONY_CL_NODE_BASE
