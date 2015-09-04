
#ifndef H_SYMPHONY_ITEM_SET
#define H_SYMPHONY_ITEM_SET

#include <list>
#include <string>
#include "has-modifiers.hpp"

class Being;
class Item;

class ItemSet: public HasModifiers {
  public:
    ItemSet(void);
    ~ItemSet(void);

    std::list<Item*>&             items(void)         { return _items; }
    const std::list<Item*>&       items(void) const   { return _items; }

    std::string   serializeItems(void) const;
    void          unserializeItems(std::string ser);

    unsigned          size(void) const;
    unsigned          completion(Being* being) const;
    bool              isComplete(Being* being) const;
    std::list<Item*>  equipped(Being* being) const;
    std::list<Item*>  unequipped(Being* being) const;
    bool              contains(Item* item) const;
    bool              willCompleteSet(Being* being, Item* item) const;
    bool              willBreakSet(Being* being, Item* item) const;

  private:
    std::list<Item*>      _items;
};

#endif // H_SYMPHONY_ITEM_SET
