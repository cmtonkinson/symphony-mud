
#ifndef H_SYMPHONY_PLACEMENT_ITEM
#define H_SYMPHONY_PLACEMENT_ITEM

#include "placement.h"

class PlacementItem: public Placement {
  public:
    // constructors...
    PlacementItem(void);
    ~PlacementItem(void);

    // public accessor methods...
    void            preposition(unsigned preposition)                   { _preposition = preposition; }
    unsigned        preposition(void) const                             { return _preposition; }
    void            indirectItem(unsigned long indirectItem)        { _indirectItem = indirectItem; }
    unsigned long   indirectItem(void) const                          { return _indirectItem; }
    void            indirectItemIndex(unsigned indirectItemIndex)   { _indirectItemIndex = indirectItemIndex; }
    unsigned        indirectItemIndex(void) const                     { return _indirectItemIndex; }

    // general methods...
    virtual std::string notes(void) const;
    virtual void        destroy(void);
    virtual bool        execute(std::list<Item*>& new_items, std::list<Npc*>& new_npcs);

  private:
    short           _preposition;         // in, on, carry, wear
    unsigned long   _indirectItem;      // vnum of whatever the item will load in or on
    short           _indirectItemIndex; // which _indirectItem gets the item (e.g. if there are more than one in the room)
};

#endif // #ifndef H_SYMPHONY_PLACEMENT_ITEM
