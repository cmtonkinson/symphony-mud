
#ifndef H_SYMPHONY_WORLD
#define H_SYMPHONY_WORLD

#include <map>
#include <set>
#include <string>
#include <vector>
#include "ability-table.h"
#include "area.h"
#include "avatar.h"
#include "board.h"
#include "exit.h"
#include "io-handler.h"
#include "mob.h"
#include "mysql.h"
#include "object.h"
#include "regex.h"
#include "room.h"
#include "schedule.h"

// Exit status...
#define EXIT_NORMAL         0
#define EXIT_FAILED_BOOT    1
#define EXIT_FAILED_REBOOT  2

// Time definitions...
#define GRAN_MINUTE   0
#define GRAN_HOUR     1
#define GRAN_DAY      2
#define GRAN_YEAR     3
#define MINUTE        60
#define HOUR          MINUTE*60
#define DAY           HOUR*24
#define YEAR          DAY*365

// Log levels...
#define SYSTEM    0
#define NOTICE    1
#define WARNING   2
#define ERROR     3
#define FATAL     4

// Log entry types...
#define LOG_SYSTEM  1
#define LOG_WORLD   2
#define LOG_AVATAR  3
#define LOG_EVENT   4
#define LOG_PLAYER  5

// Copyover junk
#define REBOOT_FILE   "sockets.copy"

class Socket;

class World {
  public:
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
    Mysql*                                                    getMysql(void)                  { return &_mysql; }
    Socket*                                                   getServer(void)                 { return &_server; }
    std::map<std::string,Avatar*>&                            getAvatars(void)                { return _avatars; }
    const std::map<std::string,Avatar*>&                      getAvatars(void) const          { return _avatars; }
    std::set<Area*,area_comp>&                                getAreas(void)                  { return _areas; }
    const std::set<Area*,area_comp>&                          getAreas(void) const            { return _areas; }
    std::set<std::pair<unsigned long,unsigned long> >&        permissions(void)               { return _permissions; }
    const std::set<std::pair<unsigned long,unsigned long> >&  permissions(void) const         { return _permissions; }
    std::set<Object*>&                                        getObjects(void)                { return _objects; }
    const std::set<Object*>&                                  getObjects(void) const          { return _objects; }
    std::set<Creature*>&                                      getCreatures(void)              { return _creatures; }
    const std::set<Creature*>&                                getCreatures(void) const        { return _creatures; }
    std::map<unsigned,Board*>&                                boards(void)                    { return _boards; }
    const std::map<unsigned,Board*>&                          boards(void) const              { return _boards; }
    InputIOHandler*                                           npcIOHandler(void)              { return &_npcIOHandler; }

    // Control
    void                  startup(void);
    void                  exist(const unsigned int& fd = 0);
    void                  turn(void);
    bool                  reboot(Creature* creature = NULL);
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
    void                  bigBrother(Creature* creature, const unsigned long& type, const char* format, ...);

    // Events
    Schedule*             schedule(void) { return &_schedule; }
    void                  handleJobs(void);
    void                  jobsPerTurn(const unsigned& limit)  { _jobsPerTurn = limit; }
    unsigned              jobsPerTurn(void) const             { return _jobsPerTurn; }
    unsigned              jobsInQueue(void) const             { return _schedule.size(); }

    // Creatures
    void                  insert(Creature* creature);
    void                  remove(Creature* creature);
    Creature*             findCreature(const std::string& name);
    Avatar*               findAvatar(const std::string& name);
    bool                  transport(Creature* creature, const unsigned long& vnum);

    // Avatars (these should be deprecated soon)
    void                  updateByName(Avatar* avatar);
    Avatar*               findAvatar(const unsigned long& ID);
    std::string           getAvatarNameByID(const unsigned long& ID);
    bool                  removeAvatar(const std::string& name);

    // Areas
    bool                  loadAreas(Mysql* db);
    bool                  saveAreas(Mysql* db);
    void                  insert(Area* area);
    void                  remove(Area* area);
    Area*                 findArea(const unsigned long& ID);
    Area*                 lookup(const unsigned long& vnum);

    // Area Permissions
    bool                  hasPermission(Area* area, Avatar* avatar);
    void                  givePermission(Area* area, Avatar* avatar);
    void                  removePermission(Area* area, Avatar* avatar);
    std::vector<Avatar*>  getPermissions(Area* area);
    std::vector<Area*>    getPermissions(Avatar* avatar);
    void                  loadPermissions(void);

    // Rooms
    Room*                 findRoom(const unsigned long& vnum);

    // Objects
    void                  insert(Object* object);
    void                  remove(Object* object);
    Object*               findObject(const std::string& name);
    bool                  transport(Object* object, Room* from, const unsigned long& vnum);

    // Boards
    void                  add(Board* board);
    void                  add(const unsigned short& board, Note* note);
    void                  saveBoards(void);

    // Static methods...
    static unsigned       rand(const unsigned& min, const unsigned& max);
    static World&         Instance(void);
    static void           worldLog(const unsigned short& level, const unsigned short& type, const char* format, ...);
    static void           playerLog(const unsigned short& level, const unsigned short& type, const char* format, ...);
    static unsigned long  now(void);
    static std::string    strnow(void);
    static std::string    strtime(time_t t = 0);
    static std::string    realtime(const unsigned long& seconds, unsigned short granularity = GRAN_DAY);
    static bool           search_map(Creature* creature, Room*** map, const unsigned short& ymax, const unsigned short& xmax, const short& y, const short& x, Room* room, std::string** display);
    static std::string    get_marker(Creature* creature, Room* room);

  private:
    time_t                                                  _booted;
    bool                                                    _exists;
    bool                                                    _copyover;
    Mysql                                                   _mysql;
    Socket                                                  _server;
    std::map<std::string,Avatar*>                           _avatars;
    std::set<Area*,area_comp>                               _areas;
    std::set<std::pair<unsigned long,unsigned long> >       _permissions;
    std::set<Object*>                                       _objects;
    std::set<Creature*>                                     _creatures;
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
