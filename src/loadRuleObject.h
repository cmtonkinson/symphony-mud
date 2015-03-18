
#ifndef H_SYMPHONY_LOADRULE_OBJECT
#define H_SYMPHONY_LOADRULE_OBJECT

#include "loadRule.h"

class LoadRuleObject: public LoadRule {
  public:
    // constructors...
    LoadRuleObject(void);
    ~LoadRuleObject(void);

    // public accessor methods...
    void            preposition(unsigned preposition)                   { _preposition = preposition; }
    unsigned        preposition(void) const                             { return _preposition; }
    void            indirectObject(unsigned long indirectObject)        { _indirectObject = indirectObject; }
    unsigned long   indirectObject(void) const                          { return _indirectObject; }
    void            indirectObjectIndex(unsigned indirectObjectIndex)   { _indirectObjectIndex = indirectObjectIndex; }
    unsigned        indirectObjectIndex(void) const                     { return _indirectObjectIndex; }

    // general methods...
    virtual std::string notes(void) const;
    virtual void        destroy(void);
    virtual bool        execute(std::list<Object*>& new_objects, std::list<Mob*>& new_mobs);

  private:
    short           _preposition;         // in, on, carry, wear
    unsigned long   _indirectObject;      // vnum of whatever the object will load in or on
    short           _indirectObjectIndex; // which _indirectObject gets the object (e.g. if there are more than one in the room)
};

#endif // #ifndef H_SYMPHONY_LOADRULE_OBJECT
