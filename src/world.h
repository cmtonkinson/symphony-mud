
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

    // Exit status...
    static const unsigned EXIT_NORMAL        = 0;
    static const unsigned EXIT_ERROR         = 1;
    static const unsigned EXIT_FAILED_BOOT   = 2;
    static const unsigned EXIT_FAILED_REBOOT = 3;
    // Time definitions...
    static const unsigned GRAN_MINUTE = 0;
    static const unsigned GRAN_HOUR   = 1;
    static const unsigned GRAN_DAY    = 2;
    static const unsigned GRAN_YEAR   = 3;
    static const unsigned MINUTE      = 60;
    static const unsigned HOUR        = MINUTE*60;
    static const unsigned DAY         = HOUR*24;
    static const unsigned YEAR        = DAY*365;
    // Log levels...
    static const unsigned LOG_LEVEL_SYSTEM  = 0;
    static const unsigned LOG_LEVEL_NOTICE  = 1;
    static const unsigned LOG_LEVEL_WARNING = 2;
    static const unsigned LOG_LEVEL_ERROR   = 3;
    static const unsigned LOG_LEVEL_FATAL   = 4;
    // Log entry types...
    static const unsigned LOG_TYPE_SYSTEM = 1;
    static const unsigned LOG_TYPE_WORLD  = 2;
    static const unsigned LOG_TYPE_AVATAR = 3;
    static const unsigned LOG_TYPE_EVENT  = 4;
    static const unsigned LOG_TYPE_PLAYER = 5;
    // Copyover junk
    static const char* REBOOT_FILE;

    // Constructors...
    World(void);
    ~World(void);

    // Accessor methods...
    void                                                      booted(const time_t& booted)    { _booted = booted; }
    time_t                                                    booted(void) const              { return _booted; }
    void                                                      exists(const bool& exists)      { _exists = exists; }
    bool                                                      exists(void) const              { return _exists; }
    void                                                      copyover(const bool& copyover)  { _copyover = copyover; }
    bool                                                      copyover(void) const            { return _copyover; }
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
    void                  turn(void);
    bool                  reboot(Being* being = NULL);
    void                  recover(const unsigned int& fd);
    bool                  load(void);
    bool                  save(void);
    bool                  save(RecurringJob* job)                                             { return save(); }
    bool                  toggleCommand(char table_prefix, std::string command_name, bool enabled);
    bool                  loadSocials(void);
    void                  loadAbilities(void);
    void                  saveSocials(void);
    bool                  tick(RecurringJob* job);

    // I/O
    void                  handleNewConnections(void);
    void                  handleInput(void);
    void                  handleOutput(void);
    void                  handleDisconnects(void);
    void                  broadcast(const std::string& message);
    void                  bigBrother(Being* being, const unsigned long& type, const char* format, ...);

    // Events
    Schedule*             schedule(void) { return &_schedule; }
    void                  handleJobs(void);
    void                  jobsPerTurn(const unsigned& limit)  { _jobsPerTurn = limit; }
    unsigned              jobsPerTurn(void) const             { return _jobsPerTurn; }
    unsigned              jobsInQueue(void) const             { return _schedule.size(); }

    // Beings
    void                  insert(Being* being);
    void                  remove(Being* being);
    Being*             findBeing(const std::string& name);
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
    Item*               findItem(const std::string& name);
    bool                  transport(Item* item, Room* from, const unsigned long& vnum);

    // Boards
    void                  add(Board* board);
    void                  add(const unsigned short& board, Note* note);
    void                  saveBoards(void);

    // Static methods...
    static unsigned       rand(const unsigned& min, const unsigned& max);
    static World&         Instance(void);
    static void           worldLog(unsigned level, unsigned type, const char* format, ...);
    static void           playerLog(unsigned level, unsigned type, const char* format, ...);
    static unsigned long  now(void);
    static std::string    strnow(void);
    static std::string    strtime(time_t t = 0);
    static std::string    realtime(const unsigned long& seconds, unsigned granularity = GRAN_DAY);
    static bool           search_map(Being* being, Room*** map, const unsigned short& ymax, const unsigned short& xmax, const short& y, const short& x, Room* room, std::string** display);
    static std::string    get_marker(Being* being, Room* room);

  private:
    time_t                                                  _booted;
    bool                                                    _exists;
    bool                                                    _copyover;
    Socket                                                  _server;
    std::map<std::string,Avatar*>                           _avatars;
    std::set<Zone*,zone_comp>                               _zones;
    std::set<Item*>                                       _items;
    std::set<Being*>                                     _beings;
    std::map<unsigned,Board*>                               _boards;
    InputIOHandler                                          _npcIOHandler;
    Schedule                                                _schedule;
    unsigned                                                _jobsPerTurn;
};

/*
 * Singleton
 */
inline World& World::Instance(void) {
  static World world;
  return world;
}

#endif // #ifndef H_SYMPHONY_WORLD
