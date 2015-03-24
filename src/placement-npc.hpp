
#ifndef H_SYMPHONY_PLACEMENT_NPC
#define H_SYMPHONY_PLACEMENT_NPC

#include "placement.h"

class PlacementNpc: public Placement {
  public:
    // constructors...
    PlacementNpc(void);
    ~PlacementNpc(void);

    // general methods...
    virtual std::string notes(void) const;
    virtual void        destroy(void);
    virtual bool        execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs);
};

#endif // #ifndef H_SYMPHONY_PLACEMENT_NPC
