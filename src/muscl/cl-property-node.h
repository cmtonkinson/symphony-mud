
#ifndef SYMPHONY_CL_PROPERTY_NODE
#define SYMPHONY_CL_PROPERTY_NODE

#include "cl-node.h"
#include "datum.h"

template <class ObjectType, class DataType>
class PropertyNode: public ContextNode {
  public:
    PropertyNode(ObjectType* target, DataType (ObjectType::*getter)(void)const, void (ObjectType::*setter)(const DataType& ref)):
        ContextNode(~0),
        _target(target),
        _getter(getter),
        _setter(setter) {
      if (!_getter) _permission &= ~CL_PERM_READ;
      if (!_setter) _permission &= ~CL_PERM_WRITE;
      return;
    }

    virtual ~PropertyNode(void) { }

    virtual bool r(Datum& value) {
      if (readable()) {
        value = (_target->*_getter)();
        return true;
      }
      return false;
    }

    virtual bool w(const Datum& value) {
      if (writable()) {
        (_target->*_setter)((DataType)value);
        return true;
      }
      return false;
    }

  private:
    ObjectType* _target;
    DataType (ObjectType::*_getter)(void)const;
    void (ObjectType::*_setter)(const DataType& ref);
};

/* These two utility functions are added as syntax-sugar so our eyes don't bleed as much casting
 * pointers to member functions to resolve linker name-based resolution.  Taken 2010/10/16 from
 * http://stackoverflow.com/questions/3946955/unable-to-resolve-overloaded-class-methods-in-template-delegate/3948560#3948560
 * Thanks, UncleBens!
 */
template <class ObjectType, class DataType>
DataType (ObjectType::*as_getter(DataType (ObjectType::*g)(void)const))() const {
  return g;
}

template <class ObjectType, class DataType>
void (ObjectType::*as_setter(void (ObjectType::*s)(const DataType&)))(const DataType&) {
  return s;
}

#endif // #ifndef SYMPHONY_CL_PROPERTY_NODE
