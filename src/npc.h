
#ifndef H_SYMPHONY_NPC
#define H_SYMPHONY_NPC

#include "being.h"

class Zone;

class Npc: public Being {
  public:

    static const unsigned MIN_NPCILITY     = 1;
    static const unsigned DEFAULT_NPCILITY = 3;
    static const unsigned MAX_NPCILITY     = 5;

    static const unsigned MIN_AGGRESSIVENESS     = 1;
    static const unsigned DEFAULT_AGGRESSIVENESS = 3;
    static const unsigned MAX_AGGRESSIVENESS     = 5;

    // constructors...
    Npc(void);
    Npc(Zone* zone, unsigned vnum);
    Npc(const Npc& ref);
    virtual ~Npc(void);

    // public accessors...
    void                vnum(unsigned vnum)                     { _vnum = vnum; }
    unsigned            vnum(void) const                        { return _vnum; }
    void                mobility(unsigned mobility);
    unsigned            mobility(void) const                    { return _mobility; }
    void                aggressiveness(unsigned aggressiveness);
    unsigned            aggressiveness(void) const              { return _aggressiveness; }

    // general methods...
    virtual bool        isNpc(void) const                 { return true; }
    virtual void        whatHappensWhenIDie(void);

    // This is the primary entry method to initiate all automated Npc behavior. It is used e.g.
    // when Npcs are placed from placements or the `nload` Command to breathe life into the Npc and
    // cause it to do things like move around the world and other randomized or programatic actions.
    void npcilize(void);

    bool auto_move(Job* job);

    // statics...
    static Npc*         create(Zone* zone, unsigned vnum);
    static Npc*         create(Npc* npc, Room* room);
    static std::string  getInformation(Npc* npc);

  private:
    // data...
    unsigned            _vnum;
    unsigned            _mobility;
    unsigned            _aggressiveness;
};

#endif // !H_SYMPHONY_NPC
