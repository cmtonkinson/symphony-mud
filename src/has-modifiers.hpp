
#ifndef H_SYMPHONY_HAS_MODIFIERS
#define H_SYMPHONY_HAS_MODIFIERS

#include <list>
#include <string>
#include "modifier.hpp"

typedef std::list<Modifier> ModifierList;

class HasModifiers {
  public:
    HasModifiers(void);
    HasModifiers(const HasModifiers& ref);
    virtual ~HasModifiers(void);

    ModifierList&          modifiers(void)          { return _modifiers; }
    const ModifierList&    modifiers(void) const    { return _modifiers; }

    std::string serializeModifiers(void) const;
    void        unserializeModifiers(std::string ser);

    void  add(Modifier mod);
    void  remove(Modifier mod);
    bool  has(Modifier mod);

    virtual void modify(Modifier mod) = 0;

  private:
    ModifierList _modifiers;
};

#endif // #ifndef H_SYMPHONY_HAS_MODIFIERS
