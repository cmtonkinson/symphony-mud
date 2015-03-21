
#ifndef H_SYMPHONY_WORLD
#define H_SYMPHONY_WORLD

#include <map>
#include <set>
#include <string>
#include <vector>
#include "ability-table.h"
#include "zone.h"
#include "avatar.h"
#include "board.h"
#include "exit.h"
#include "io-handler.h"
#include "npc.h"
#include "item.h"
#include "regex.h"
#include "room.h"
#include "schedule.h"

class Socket;

class World {
  public:
    World(void);
    ~World(void);

    // Accessor methods...
    void                                                      booted(const time_t& booted)    { _booted = booted; }
    time_t                                                    booted(void) const              { return _booted; }
    void                                                      exists(const bool& exists)      { _exists = exists; }
    bool                                                      exists(void) const              { return _exists; }
    void                                                      rebooting(bool rebooting)       { _rebooting = rebooting; }
    bool                                                      rebooting(void) const           { return _rebooting; }
    Socket*                                                   getServer(void)                 { return &_server; }
    std::map<std::string,Avatar*>&                            getAvatars(void)                { return _avatars; }
    const std::map<std::string,Avatar*>&                      getAvatars(void) const          { return _avatars; }
    std::set<Zone*,zone_comp>&                                getZones(void)                  { return _zones; }
    const std::set<Zone*,zone_comp>&                          getZones(void) const            { return _zones; }
    std::set<Item*>&                                        getItems(void)                { return _items; }
    const std::set<Item*>&                                  getItems(void) const          { return _items; }
    std::set<Being*>&                                      getBeings(void)              { return _beings; }
    const std::set<Being*>&                                getBeings(void) const        { return _beings; }
    std::map<unsigned,Board*>&                                boards(void)                    { return _boards; }
    const std::map<unsigned,Board*>&                          boards(void) const              { return _boards; }
    InputIOHandler*                                           npcIOHandler(void)              { return &_npcIOHandler; }

    // Control
    void                  startup(void);
    void                  exist(const unsigned int& fd = 0);
    void                  tick(void);
    bool                  reboot(Being* being = NULL);
    void                  recover(const unsigned int& fd);
    bool                  load(void);
    bool                  save(void);
    bool                  save(RecurringJob* job)                                             { return save(); }
    bool                  toggleCommand(char table_prefix, std::string command_name, bool enabled);
    bool                  loadSocials(void);
    void                  loadAbilities(void);
    void                  saveSocials(void);
    bool                  tock(RecurringJob* job);

    // I/O
    void                  handleNewConnections(void);
    void                  handleInput(void);
    void                  handleOutput(void);
    void                  handleDisconnects(void);
    void                  broadcast(const std::string& message);
    void                  bigBrother(Being* being, unsigned level, std::string message);

    // Events
    Schedule*             schedule(void) { return &_schedule; }
    void                  handleJobs(void);
    void                  jobsPerTurn(const unsigned& limit)  { _jobsPerTurn = limit; }
    unsigned              jobsPerTurn(void) const             { return _jobsPerTurn; }
    unsigned              jobsInQueue(void) const             { return _schedule.size(); }

    // Beings
    void                  insert(Being* being);
    void                  remove(Being* being);
    Being*                findBeing(const std::string& name);
    Avatar*               findAvatar(const std::string& name);
    bool                  transport(Being* being, const unsigned long& vnum);

    // Avatars (these should be deprecated soon)
    void                  updateByName(Avatar* avatar);
    Avatar*               findAvatar(const unsigned long& ID);
    bool                  removeAvatar(const std::string& name);

    // Zones
    void                  loadZones(void);
    void                  insert(Zone* zone);
    void                  remove(Zone* zone);
    Zone*                 findZone(const unsigned long& ID);
    Zone*                 lookup(const unsigned long& vnum);

    void                  loadDisabledCommands(void);

    // Rooms
    Room*                 findRoom(const unsigned long& vnum);

    // Items
    void                  insert(Item* item);
    void                  remove(Item* item);
    Item*                 findItem(const std::string& name);
    bool                  transport(Item* item, Room* from, const unsigned long& vnum);

    // Boards
    void                  add(Board* board);
    void                  add(const unsigned short& board, Note* note);
    void                  saveBoards(void);

    // Static methods...
    static World&         Instance(void);
    static bool           search_map(Being* being, Room*** map, const unsigned short& ymax, const unsigned short& xmax, const short& y, const short& x, Room* room, std::string** display);
    static std::string    get_marker(Being* being, Room* room);

  private:
    time_t                          _booted;
    bool                            _exists;
    bool                            _rebooting;
    Socket                          _server;
    std::map<std::string,Avatar*>   _avatars;
    std::set<Zone*,zone_comp>       _zones;
    std::set<Item*>                 _items;
    std::set<Being*>                _beings;
    std::map<unsigned,Board*>       _boards;
    InputIOHandler                  _npcIOHandler;
    Schedule                        _schedule;
    unsigned                        _jobsPerTurn;
};

/*
 * Singleton
 */
inline World& World::Instance(void) {
  static World world;
  return world;
}

#endif // #ifndef H_SYMPHONY_WORLD
