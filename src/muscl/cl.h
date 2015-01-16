
#ifndef SYMPHONY_CL
#define SYMPHONY_CL

#include <map>
#include <string>
#include "datum.h"
#include "cl-node.h"

using std::map;
using std::string;

class ContextLayer {
  public:
    typedef map<string,ContextNode*> ContextMap;

    ContextLayer(void);
    ~ContextLayer(void);

    bool  isValidReference(const string& name);

    bool  readable(const string& name) const;
    bool  writable(const string& name) const;

    bool  read(const string& name, Datum& value);
    bool  write(const string& name, const Datum& value);

    bool          add(const string& name, ContextNode* node);
    bool          remove(const string& name);
    ContextNode*  resolve(const string& name) const;

  private:
    ContextMap  _objects;
};

#endif // #ifndef SYMPHONY_CL
