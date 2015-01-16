
#ifndef H_SYMPHONY_CONTAINER_LIST
#define H_SYMPHONY_CONTAINER_LIST

#include <list>
#include <string>
#include "container.h"
#include "identifiers.h"

class Object;

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
    virtual void                add(Object* object);
    virtual void                add(const std::list<Object*>& objects);
    virtual void                remove(Object* object);
    virtual void                remove(const std::list<Object*>& objects);
    virtual void                purgeObjects(void);
    virtual std::list<Object*>  searchObjects(const std::string& q);
    virtual Object*             searchSingleObject(const std::string& q);

    // general methods...
    std::list<Object*>  searchObjects(const unsigned long& vnum);
    unsigned            howManyObjects(const unsigned long& vnum) const;
    std::string         listObjects(bool compact = true) const;

    // public accessor methods...
    std::list<Object*>&                                   objectList(void)        { return _objectList; }
    const std::list<Object*>&                             objectList(void) const  { return _objectList; }
    std::string (Identifiers::*getName(void))(void) const                           { return _getName; }

  private:
    std::list<Object*> _objectList;
};

#endif // H_SYMPHONY_CONTAINER_LIST
