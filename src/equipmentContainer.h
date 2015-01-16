
#ifndef H_SYMPHONY_EQUIPMENT_CONTAINER
#define H_SYMPHONY_EQUIPMENT_CONTAINER

#include <list>
#include <map>
#include <string>
#include "container.h"
#include "identifiers.h"

class Object;

class EquipmentContainer: public Container {
  public:
    // constructors...
    EquipmentContainer(void);
    EquipmentContainer(const EquipmentContainer& ref);
    virtual ~EquipmentContainer(void);

    // virtual methods....
    virtual void                add(Object* object, const int& location);
    virtual void                remove(Object* object);
    virtual void                remove(const std::list<Object*>& objects);
    virtual void                purgeObjects(void);
    virtual std::list<Object*>  searchObjects(const std::string& q);
    virtual Object*             searchSingleObject(const std::string& q);

    // general methods...
    std::string                 listObjects(void) const;
    Object*                     at(int location);

    // public accessor methods...
    std::map<int,Object*>&                                  objectMap(void)       { return _objectMap; }
    const std::map<int,Object*>&                            objectMap(void) const { return _objectMap; }

  private:
    std::map<int,Object*> _objectMap;
};

#endif // !H_SYMPHONY_CONTAINER_MAP
