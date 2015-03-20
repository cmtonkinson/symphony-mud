
#ifndef H_SYMPHONY_OBJECT_FURNITURE
#define H_SYMPHONY_OBJECT_FURNITURE

#include "enumTable.h"

class Being;

class ObjFurniture {
  public:
    // constructors...
    ObjFurniture(void);
    ObjFurniture(const ObjFurniture& ref);
    ~ObjFurniture(void);

    // public accessors
    void      capacity(const unsigned& capacity)      { _capacity = capacity; }
    unsigned  capacity(void) const                    { return _capacity; }
    void      layOn(const unsigned& layOn)            { _layOn = layOn; }
    unsigned  layOn(void) const                       { return _layOn; }
    void      sitAt(const unsigned& sitAt)            { _sitAt = sitAt; }
    unsigned  sitAt(void) const                       { return _sitAt; }
    void      sitOn(const unsigned& sitOn)            { _sitOn = sitOn; }
    unsigned  sitOn(void) const                       { return _sitOn; }
    void      standOn(const unsigned& standOn)        { _standOn = standOn; }
    unsigned  standOn(void) const                     { return _standOn; }
    void      current(const unsigned& current)        { _current = current; }
    unsigned  current(void) const                     { return _current; }
    std::map<EnumInt,std::list<Being*> >&        beings(void)       { return _beings; }
    const std::map<EnumInt,std::list<Being*> >&  beings(void) const { return _beings; }

    // general methods
    bool  add(Being* being, const EnumInt& position);
    bool  remove(Being* being);

  private:
    unsigned _capacity; // how many, in total, can be using this at once?
    unsigned _layOn;    // how many can lay on this?
    unsigned _sitAt;    // how many can sit at this?
    unsigned _sitOn;    // how many can sit on this?
    unsigned _standOn;  // how many can stand on this?
    unsigned _current;  // how many, in total, are currently here?
    std::map<EnumInt,std::list<Being*> > _beings; // who's here?
};

#endif // H_SYMPHONY_OBJECT_FURNITURE
