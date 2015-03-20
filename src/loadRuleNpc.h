
#ifndef H_SYMPHONY_LOADRULE_NPC
#define H_SYMPHONY_LOADRULE_NPC

#include "loadRule.h"

class LoadRuleNpc: public LoadRule {
  public:
    // constructors...
    LoadRuleNpc(void);
    ~LoadRuleNpc(void);

    // general methods...
    virtual std::string notes(void) const;
    virtual void        destroy(void);
    virtual bool        execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs);
};

#endif // #ifndef H_SYMPHONY_LOADRULE_NPC
