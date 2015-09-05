
#ifndef H_SYMPHONY_ITEM_CONTAINER
#define H_SYMPHONY_ITEM_CONTAINER

#include "item.hpp"

typedef std::list<Item*> ItemList;

class ItemContainer {
  public:
    ItemContainer(void);
    ItemContainer(const ItemContainer& ref);
    virtual ~ItemContainer(void);

    ItemList&       items(void)         { return _items; }
    const ItemList& items(void) const   { return _items; }

    void    add(Item* item);
    void    remove(Item* item);
    bool    has(Item* item) const;

  private:
    ItemList  _items;
};

#endif // #ifndef H_SYMPHONY_ITEM_CONTAINER
