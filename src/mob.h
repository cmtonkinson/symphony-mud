
#ifndef H_SYMPHONY_MOB
#define H_SYMPHONY_MOB

#include "being.h"

class Zone;

class Mob: public Being {
  public:

    static const unsigned MIN_MOBILITY     = 1;
    static const unsigned DEFAULT_MOBILITY = 3;
    static const unsigned MAX_MOBILITY     = 5;

    static const unsigned MIN_AGGRESSIVENESS     = 1;
    static const unsigned DEFAULT_AGGRESSIVENESS = 3;
    static const unsigned MAX_AGGRESSIVENESS     = 5;

    // constructors...
    Mob(void);
    Mob(Zone* zone, unsigned vnum);
    Mob(const Mob& ref);
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
    virtual void        whatHappensWhenIDie(void);

    // This is the primary entry method to initiate all automated Mob behavior. It is used e.g.
    // when Mobs are placed from load rules or the `mload` Command to breathe life into the Mob and
    // cause it to do things like move around the world and other randomized or programatic actions.
    void mobilize(void);

    bool auto_move(Job* job);

    // statics...
    static Mob*         create(Zone* zone, unsigned vnum);
    static Mob*         create(Mob* mob, Room* room);
    static std::string  getInformation(Mob* mob);

  private:
    // data...
    unsigned            _vnum;
    unsigned            _mobility;
    unsigned            _aggressiveness;
};

#endif // !H_SYMPHONY_MOB
