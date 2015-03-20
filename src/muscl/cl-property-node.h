
#ifndef SYMPHONY_CL_PROPERTY_NODE
#define SYMPHONY_CL_PROPERTY_NODE

#include "cl-node.h"
#include "datum.h"

template <class ItemType, class DataType>
class PropertyNode: public ContextNode {
  public:
    PropertyNode(ItemType* target, DataType (ItemType::*getter)(void)const, void (ItemType::*setter)(const DataType& ref)):
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
    ItemType* _target;
    DataType (ItemType::*_getter)(void)const;
    void (ItemType::*_setter)(const DataType& ref);
};

/* These two utility functions are added as syntax-sugar so our eyes don't bleed as much casting
 * pointers to member functions to resolve linker name-based resolution.  Taken 2010/10/16 from
 * http://stackoverflow.com/questions/3946955/unable-to-resolve-overloaded-class-methods-in-template-delegate/3948560#3948560
 * Thanks, UncleBens!
 */
template <class ItemType, class DataType>
DataType (ItemType::*as_getter(DataType (ItemType::*g)(void)const))() const {
  return g;
}

template <class ItemType, class DataType>
void (ItemType::*as_setter(void (ItemType::*s)(const DataType&)))(const DataType&) {
  return s;
}

#endif // #ifndef SYMPHONY_CL_PROPERTY_NODE
