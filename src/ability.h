#ifndef H_SYMPHONY_ABILITY
#define H_SYMPHONY_ABILITY

#include <set>
#include <string>
#include "enumTable.h"

class Creature;

class Ability {
  public:
    Ability(void);
    virtual ~Ability(void);

    std::set<Ability*>&         dependencies(void)        { return _dependencies; }
    const std::set<Ability*>&   dependencies(void) const  { return _dependencies; }
    std::set<Ability*>&         dependents(void)          { return _dependents; }
    const std::set<Ability*>&   dependents(void) const    { return _dependents; }

    void                  add_dependency(Ability* ability);
    void                  add_dependent(Ability* ability);
    bool                  has_dependencies(void) const;
    bool                  has_dependents(void) const;

    bool                  is_root(void) const;
    bool                  is_leaf(void) const;

    void                  name(std::string name)  { _name = name; }
    std::string           name(void)              { return _name; }
    void                  level(unsigned level)   { _level = level; }
    unsigned              level(void) const       { return _level; }

    virtual void          setup(void);
    virtual bool          accessible(Creature* creature) const;

  private:

    std::string           _name;
    unsigned              _level;
    std::set<Ability*>    _dependencies;
    std::set<Ability*>    _dependents;

};

#define DEF_ABILITY(NAME,CLASS)           \
class CLASS: public Ability {             \
  public:                                 \
    CLASS(unsigned level_) {              \
      name(NAME);                         \
      level(level_);                      \
      return;                             \
    }                                     \
    virtual ~CLASS(void) { return; }      \
};                                        \

#endif // #ifndef H_SYMPHONY_ABILITY
