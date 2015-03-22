
#ifndef H_SYMPHONY_CONTAINER_LIST
#define H_SYMPHONY_CONTAINER_LIST

#include <list>
#include <string>
#include "container.h"
#include "identifiers.h"

class Item;

/*
 * The reason for the getName pointer is that sometimse it's necessary to to display a different
 * identifier when producing a list of Items. For example, a Being inventory will display the
 * shortname() of an Item, while a Room inventory will display the longname().
 */
class InventoryContainer: public Container {
  public:
    // constructors...
    InventoryContainer(std::string (Identifiers::*getName)(void) const);
    InventoryContainer(const InventoryContainer& ref);
    virtual ~InventoryContainer(void);

    // Hack-job because I can't get getName() to return the method pointer
    // from within the copy constructor
    std::string (Identifiers::*_getName)(void) const;

    // virtual methods....
    virtual void                add(Item* item);
    virtual void                add(const std::list<Item*>& items);
    virtual void                remove(Item* item);
    virtual void                remove(const std::list<Item*>& items);
    virtual void                purgeItems(void);
    virtual std::list<Item*>  searchItems(const std::string& q);
    virtual Item*             searchSingleItem(const std::string& q);

    // general methods...
    std::list<Item*>  searchItems(const unsigned long& vnum);
    unsigned            howManyItems(const unsigned long& vnum) const;
    std::string         listItems(bool compact = true) const;

    // public accessor methods...
    std::list<Item*>&                                   itemList(void)        { return _itemList; }
    const std::list<Item*>&                             itemList(void) const  { return _itemList; }
    std::string (Identifiers::*getName(void))(void) const                         { return _getName; }

  private:
    std::list<Item*> _itemList;
};

#endif // H_SYMPHONY_CONTAINER_LIST
