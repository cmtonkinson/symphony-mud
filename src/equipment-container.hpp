
#ifndef H_SYMPHONY_EQUIPMENT_CONTAINER
#define H_SYMPHONY_EQUIPMENT_CONTAINER

#include <list>
#include <map>
#include <string>
#include "container.h"
#include "identifiers.h"

class Item;

class EquipmentContainer: public Container {
  public:
    // constructors...
    EquipmentContainer(void);
    EquipmentContainer(const EquipmentContainer& ref);
    virtual ~EquipmentContainer(void);

    // virtual methods....
    virtual void                add(Item* item, const int& location);
    virtual void                remove(Item* item);
    virtual void                remove(const std::list<Item*>& items);
    virtual void                purgeItems(void);
    virtual std::list<Item*>  searchItems(const std::string& q);
    virtual Item*             searchSingleItem(const std::string& q);

    // general methods...
    std::string                 listItems(void) const;
    Item*                     at(int location);

    // public accessor methods...
    std::map<int,Item*>&                                  itemMap(void)       { return _itemMap; }
    const std::map<int,Item*>&                            itemMap(void) const { return _itemMap; }

  private:
    std::map<int,Item*> _itemMap;
};

#endif // !H_SYMPHONY_CONTAINER_MAP
