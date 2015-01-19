
#ifndef H_SYMPHONY_MOB
#define H_SYMPHONY_MOB

#include "creature.h"
#include "mysql.h"

class Area;

class Mob: public Creature {
  public:
    // constructors...
    Mob(void);
    Mob(Area* area, unsigned vnum);
    Mob(const Mob& ref);
    Mob(ROW row);
    virtual ~Mob(void);

    // public accessors...
    void                vnum(unsigned vnum)               { _vnum = vnum; }
    unsigned            vnum(void) const                  { return _vnum; }

    // general methods...
    virtual bool        isMob(void) const                 { return true; }

    // Combat...
    virtual void        whatHappensWhenIDie(void);

    // pure virtual overrides...
    virtual bool        save(void);
    virtual bool        destroy(void);

    // statics...
    static Mob*         create(Area* area, unsigned vnum);
    static Mob*         create(ROW row);
    static Mob*         create(Mob* mob, Room* room);
    static std::string  getInformation(Mob* mob);

  private:
    // data...
    unsigned       _vnum;
};

#endif // !H_SYMPHONY_MOB
