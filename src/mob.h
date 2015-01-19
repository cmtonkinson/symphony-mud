
#ifndef H_SYMPHONY_MOB
#define H_SYMPHONY_MOB

#include "creature.h"
#include "mysql.h"

class Area;

class Mob: public Creature {
  public:

    static const unsigned MIN_MOBILITY     = 1;
    static const unsigned DEFAULT_MOBILITY = 3;
    static const unsigned MAX_MOBILITY     = 5;

    static const unsigned MIN_AGGRESSIVENESS     = 1;
    static const unsigned DEFAULT_AGGRESSIVENESS = 3;
    static const unsigned MAX_AGGRESSIVENESS     = 5;

    // constructors...
    Mob(void);
    Mob(Area* area, unsigned vnum);
    Mob(const Mob& ref);
    Mob(ROW row);
    virtual ~Mob(void);

    // public accessors...
    void                vnum(unsigned vnum)                     { _vnum = vnum; }
    unsigned            vnum(void) const                        { return _vnum; }
    void                mobility(unsigned mobility);
    unsigned            mobility(void) const                    { return _mobility; }
    void                aggressiveness(unsigned aggressiveness);
    unsigned            aggressiveness(void) const              { return _aggressiveness; }

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
    unsigned            _vnum;
    unsigned            _mobility;
    unsigned            _aggressiveness;
};

#endif // !H_SYMPHONY_MOB
