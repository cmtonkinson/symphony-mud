
#ifndef H_SYMPHONY_KLASS
#define H_SYMPHONY_KLASS

#include <string>
#include "ability.hpp"
#include "ability-table.hpp"

/*
 * Klass base class - not pure virtual, but shouldn't be instantiated directly.
 */
class Klass {
  public:

    Klass(void);
    virtual ~Klass(void);

    std::string           name(void) const        { return _name; }
    const AbilityTable&   abilities(void) const   { return _abilities; }

  protected:
    std::string     _name;
    AbilityTable    _abilities;

};

/*
 * Macro for easily defining new Klasses.
 */
#define DEF_KLASS(NAME)                   \
class NAME: public Klass {                \
  public:                                 \
    NAME(void);                           \
    virtual ~NAME(void) { }               \
                                          \
    static inline NAME& Instance(void) {  \
      static NAME singleton;              \
      return singleton;                   \
    }                                     \
};                                        \

/*
 * Specific Klass definitions.
 */
DEF_KLASS(Cleric)
DEF_KLASS(Mage)
DEF_KLASS(Rogue)
DEF_KLASS(Warrior)

#endif // #ifndef H_SYMPHONY_KLASS
