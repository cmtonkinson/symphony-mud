
#ifndef H_SYMPHONY_LOADRULE_MOB
#define H_SYMPHONY_LOADRULE_MOB

#include "loadRule.h"

class LoadRuleMob: public LoadRule {
  public:
    // constructors...
    LoadRuleMob(void);
    ~LoadRuleMob(void);

    // general methods...
    virtual std::string notes(void) const;
    virtual void        destroy(void);
    virtual bool        execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs);
};

#endif // #ifndef H_SYMPHONY_LOADRULE_MOB
