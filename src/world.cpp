
#include <cstdarg>
#include <dirent.h>
#include "commandTable-default.h"
#include "commandTable.h"
#include "os.h"
#include "storage.h"
#include "world.h"

const char* World::REBOOT_FILE = "sockets.copy";

World::World(void) {
  // basic setup
  booted(time(NULL));
  exists(true);
  copyover(false);
  jobsPerTurn(5);
  schedule()->add(new RecurringJob(this, &World::tick, 50, 70));
  schedule()->add(new RecurringJob(this, &World::save, 900));
  return;
}

World::~World(void) {
  return;
}

/************************************************************ CONTROL ************************************************************/
void World::startup(void) {

  if (!load()) {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_WORLD, "There was a problem loading a world component.");
  }

  npcIOHandler()->commandTable(&(Commands::Instance()));
  exist();

  return;
}

void World::exist(const unsigned int& fd) {
  try {
    if (copyover()) {
      getServer()->setFd(fd);
    } else {
      getServer()->startup(6501);
    }
  } catch (SocketException se) {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_SYSTEM, "Couldn't start Socket server: %s", se.getError().c_str());
    exit(EXIT_FAILED_BOOT);
  }

  if (copyover()) {
    worldLog(LOG_LEVEL_SYSTEM, LOG_TYPE_SYSTEM, "System rebooted successfully.");
  } else {
    worldLog(LOG_LEVEL_SYSTEM, LOG_TYPE_SYSTEM, "System up on port %d.", getServer()->getPort());
  }

  while (exists()) {
    turn();
  }

  if (!save()) {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_WORLD, "There was a problem saving a world component.");
  }

  worldLog(LOG_LEVEL_SYSTEM, LOG_TYPE_SYSTEM, "System shutting down.");

  return;
}

void World::turn(void) {
  handleNewConnections();
  handleJobs();
  handleInput();
  handleOutput();
  handleDisconnects();
  usleep(1000); // to throttle the CPU usage
  return;
}

bool World::reboot(Being* being) {
  std::map<std::string,Avatar*>::iterator it;
  estring fd;
  FILE* fp = NULL;

  fp = fopen(REBOOT_FILE, "w");

  if (!fp) {
    if (being) {
      being->send("Copyover file couldn't be opened for writing! Copyover aborted!");
    }
    worldLog(World::LOG_LEVEL_ERROR, World::LOG_TYPE_SYSTEM, "Copyover file couldn't be opened for writing! Copyover aborted!");
    return false;
  }

  for (it = getAvatars().begin(); it != getAvatars().end(); ++it) {
    if (it->second->isConnected()) {
      fprintf(fp, "%d %s %s\n",
        it->second->socket()->getFd(),
        it->second->socket()->getIP().c_str(),
        it->second->name()
      );
    }
  }

  fclose(fp);

  broadcast("Hold on to your pants; Copyover in progress...");
  handleOutput();

  if (save()) {
    worldLog(LOG_LEVEL_SYSTEM, LOG_TYPE_SYSTEM, "World saved pre-reboot.");
  } else {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_WORLD, "There was a problem saving a world component.");
    broadcast("Copyover aborted.");
    return false;
  }

  worldLog(LOG_LEVEL_SYSTEM, LOG_TYPE_SYSTEM, "System going down for reboot.");

  fd = estring("-fd=").append(estring(getServer()->getFd()));
  if (execl("bin/symphony", "-copyover", fd.c_str(), NULL) < 0) {
    worldLog(World::LOG_LEVEL_ERROR, World::LOG_TYPE_SYSTEM, "execl(): %s", strerror(errno));
    broadcast("Copyover failed.");
  }

  return false;
}

void World::recover(const unsigned int& fd) {
  FILE* fp = NULL;
  int client_fd = 0;
  char ip[Socket::MAX_BUFFER];
  char name[Socket::MAX_BUFFER];
  Avatar* avatar = NULL;

  if (!load()) {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_WORLD, "There was a problem loading a world component.");
  }

  fp = fopen(REBOOT_FILE, "r");

  if (!fp) {
    worldLog(World::LOG_LEVEL_FATAL, World::LOG_TYPE_SYSTEM, "Copyover file couldn't be opened for reading. Copyover failed.");
    exit(EXIT_FAILED_REBOOT);
  }

  while (1) {
    if (fscanf(fp, "%d %s %s", &client_fd, ip, name) < 3) {
      break;
    }
    avatar = new Avatar(new Socket(client_fd));
    avatar->socket()->setIP(ip);
    avatar->identifiers().shortname(name);
    avatar->load();
    avatar->pushIOHandler(new InputIOHandler(avatar));
    insert(avatar);
    avatar->status().set(CONNECTED);
    avatar->restoreRoom();
    avatar->send("\n\n");
  }

  fclose(fp);
  unlink(REBOOT_FILE);

  broadcast("\n\nCopyover successful. You may now drop your pants.");

  exist(fd);

  return;
}

bool World::load(void) {
  bool status = true;

  loadZones();
  loadDisabledCommands();

  if (!loadSocials()) {
    status = false;
  }

  add(new Board(Board::GENERAL));
  add(new Board(Board::CHANGES));
  add(new Board(Board::ADMIN));
  add(new Board(Board::NEWS));
  add(new Board(Board::IMMORTAL));
  add(new Board(Board::BUILDING));

  return status;
}

bool World::save(void) {
  bool status = true;

  for (auto iter : getZones()) iter->save();

  for (std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ++a_it) {
    if (a_it->second->isConnected()) {
      a_it->second->save();
    }
  }

  saveSocials();

  worldLog(World::LOG_LEVEL_SYSTEM, World::LOG_TYPE_WORLD, "World::save()");

  return status;
}

bool World::toggleCommand(char table_prefix, std::string command_name, bool enabled) {
  CommandTable* table  = NULL;
  Command* command     = NULL;
  std::string filepath = "data/disabled_commands/";

  switch (table_prefix) {
    case 'x': table = &(Commands::Instance());      break;
    case 'A': table = &(ZeditCommands::Instance()); break;
    case 'M': table = &(NeditCommands::Instance()); break;
    case 'O': table = &(IeditCommands::Instance()); break;
    case 'P': table = &(PeditCommands::Instance()); break;
    case 'R': table = &(ReditCommands::Instance()); break;
    case 'T': table = &(TeditCommands::Instance()); break;
    default:  table = NULL;                         break;
  }

  if (table) {
    if ((command = table->find(command_name)) != NULL) {
      if (command->level() >= Being::CREATOR) return false;

      filepath << table_prefix << "_" << command->name();
      command->enabled(enabled);
      if (command->enabled()) {
        // remove the file
        unlink(filepath.c_str());
      } else {
        // touch the file
        fclose(fopen(filepath.c_str(), "w"));
      }
      return true;
    }
  }

  return false;

}

bool World::loadSocials(void) {
  for (auto iter : os::glob(os::social_glob_pattern())) SocialCommand::load(iter);
  return true;
}

void World::saveSocials(void) {
  for (auto iter : Commands::Instance().commands()) {
    if (!iter->social()) continue;
    dynamic_cast<SocialCommand*>(iter)->save();
  }
  return;
}

bool World::tick(RecurringJob* job) {
  bigBrother(NULL, ADMIN_BIGBRO_EVENTS, "Tick!");
  return true;
}

/************************************************************ I/O ************************************************************/
void World::handleInput(void) {
  std::map<std::string,Avatar*>::iterator next;
  for (std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end();) {
    next = a_it;
    next++;
    try {
      if (a_it->second->hasInput()) {
        a_it->second->handle();
      }
    } catch (SocketException se) {
      bigBrother(a_it->second, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in World::handleInput() -> %s", a_it->second->identifiers().shortname().c_str(), se.getError().c_str());
    }
    a_it = next;
  }
  return;
}

void World::handleOutput(void) {
  std::map<std::string,Avatar*>::iterator next;
  for (std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end(); ++a_it) {
    try {
      if (a_it->second->hasOutput()) {
        a_it->second->flushOutput();
      }
    } catch (SocketException se) {
      bigBrother(a_it->second, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in World::handleOutput() -> %s", a_it->second->identifiers().shortname().c_str(), se.getError().c_str());
    }
  }
  return;
}

void World::handleNewConnections(void) {
  std::map<std::string,Avatar*>::iterator a_it, next, foo;
  std::vector<Socket*> new_connections = getServer()->accept();
  std::vector<Socket*>::iterator sock_it;
  Avatar* avatar = NULL;
  for (sock_it = new_connections.begin(); !new_connections.empty();) {
    avatar = new Avatar(*sock_it);
    insert(avatar);
    avatar->pushIOHandler(new LoginBannerIOHandler(avatar));
    bigBrother(NULL, ADMIN_BIGBRO_SYSTEM, "New connection from %s!", avatar->socket()->getIP().c_str());
    new_connections.erase(sock_it);
  }
  return;
}

void World::handleDisconnects(void) {
  std::map<std::string,Avatar*>::iterator next;
  for (std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end();) {
    if (a_it->second->disconnected()) {
      /* We have to go through a bit of a song and dance here, because map::erase() does
       * not return the next valid iterator in the sequence like vector::erase() does.
       */
      next = a_it;
      ++next;
      bigBrother(a_it->second, ADMIN_BIGBRO_LOGINS, "%s has logged out from %s.", a_it->second->identifiers().shortname().c_str(), a_it->second->socket()->getIP().c_str());
      playerLog(World::LOG_LEVEL_NOTICE, World::LOG_TYPE_PLAYER, "%s (%lu) logged in from %s", a_it->second->identifiers().shortname().c_str(), a_it->second->ID(), a_it->second->socket()->getIP().c_str());
      if (a_it->second->room()) {
        a_it->second->room()->send_cond("$p has left the realm.", a_it->second);
        a_it->second->room()->remove(a_it->second);
      }
      remove(a_it->second);
      if (a_it->second->shouldDestroy()) { // permanently delete this account
        a_it->second->destroy();
      } else { // just logging out
        delete a_it->second;
      }
      a_it = next;
    } else {
      ++a_it;
    }
  }
  return;
}

void World::broadcast(const std::string& message) {
  for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); it++) {
    if (it->second->isConnected()) {
      it->second->send(message);
      it->second->send("\n");
    }
  }
  return;
}

void World::bigBrother(Being* being, const unsigned long& type, const char* format, ...) {
  char buffer[Socket::MAX_BUFFER];
  va_list args;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  if (being && type) {
    for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
      if (it->second->isConnected() && it->second->adminFlags().test(ADMIN_BIGBROTHER) && it->second->adminFlags().test(type) && it->second->level() >= being->level() && it->second != being) {
        it->second->send("BigBrother: %s\n", buffer);
      }
    }
  } else if (type) {
    for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
      if (it->second->isConnected() && it->second->adminFlags().test(ADMIN_BIGBROTHER) && it->second->adminFlags().test(type)) {
        it->second->send("BigBrother: %s\n", buffer);
      }
    }
  } else {
    for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
      if (it->second->isConnected() && it->second->adminFlags().test(ADMIN_BIGBROTHER)) {
        it->second->send("BigBrother: %s\n", buffer);
      }
    }
  }

  return;
}

/************************************************************ JOBS ************************************************************/
void World::handleJobs(void) {
  unsigned count = 0;
  while (schedule()->fire()) {
    if (++count >= jobsPerTurn()) {
      break;
    }
  }
  return;
}

/************************************************************ BEINGS ************************************************************/
void World::insert(Being* being) {
fprintf(stderr, "World::insert(%s)\n", being->name());
  getBeings().insert(being);
  if (being->isAvatar()) {
fprintf(stderr, "  -> avatar\n");
    getAvatars().insert(std::make_pair(being->name(), dynamic_cast<Avatar*>(being)));
  }
  return;
}

void World::remove(Being* being) {
fprintf(stderr, "World::remove(%s)\n", being->name());
  getBeings().erase(being);
  if (being->isAvatar()) {
fprintf(stderr, "  -> avatar\n");
    getAvatars().erase(being->name());
  }
  return;
}

Being* World::findBeing(const std::string& name) {
  for (std::set<Being*>::iterator it = getBeings().begin(); it != getBeings().end(); ++it) {
    if ((*it)->identifiers().matchesKeyword(name)) {
      if ((*it)->isAvatar()) {
        if (((Avatar*)*it)->isConnected()) {
          return *it;
        }
      } else {
        return *it;
      }
    }
  }
  return NULL;
}

/* Avatar-specific search method (slightly more efficient when searching only for PCs) */
Avatar* World::findAvatar(const std::string& name) {
  for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
    if (it->second->isConnected() && it->second->identifiers().matchesKeyword(name)) {
      return it->second;
    }
  }
  return NULL;
}

bool World::transport(Being* being, const unsigned long& vnum) {
  Room* room = NULL;
  CmdLook look;
  std::vector<std::string> look_args(1);

  if ((room = findRoom(vnum)) == NULL) {
    return false;
  }

  being->room()->remove(being);
  room->add(being);
  being->room(room);
  being->send("You've been transported to %s.\n", being->room()->name().c_str());
  look.execute(being, look_args);
  return true;
}

/************************************************************ AVATARS ************************************************************/
Avatar* World::findAvatar(const unsigned long& ID) {
  for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
    if (it->second->isConnected() && it->second->ID() == ID) {
      return it->second;
    }
  }
  return NULL;
}

bool World::removeAvatar(const std::string& name) {
  for (std::map<std::string,Avatar*>::iterator it = getAvatars().begin(); it != getAvatars().end(); ++it) {
    if (it->first == name) {
      getBeings().erase(it->second);
      getAvatars().erase(name);
      return true;
    }
  }
  return false;
}

/************************************************************ ZONES ************************************************************/
void World::loadZones(void) {
  for (auto iter : os::glob(os::zone_glob_pattern())) Zone::load(iter);
  return;
}

void World::insert(Zone* zone) {
  getZones().insert(zone);
  return;
}

void World::remove(Zone* zone) {
  getZones().erase(zone);
  return;
}

Zone* World::findZone(const unsigned long& ID) {
  for (std::set<Zone*,zone_comp>::iterator it = getZones().begin(); it != getZones().end(); ++it) {
    if ((*it)->ID() == ID) {
      return *it;
    }
  }
  return NULL;
}

Zone* World::lookup(const unsigned long& vnum) {
  for (std::set<Zone*,zone_comp>::iterator it = getZones().begin(); it != getZones().end(); ++it) {
    if ((*it)->low() <= vnum && vnum <= (*it)->high()) {
      return *it;
    }
  }
  return NULL;
}

void World::loadDisabledCommands(void) {
  for (auto iter : os::glob(os::disabled_command_glob_pattern())) {
    toggleCommand(iter[0], iter.substr(2), false);
  }
  return;
}

/************************************************************ ROOMS ************************************************************/
Room* World::findRoom(const unsigned long& vnum) {
  std::map<unsigned long,Room*>::iterator r_it;
  std::map<unsigned long,Room*>::iterator r_end;
  for (std::set<Zone*,zone_comp>::iterator a_it = getZones().begin(); a_it != getZones().end(); ++a_it) {
    r_end = (*a_it)->rooms().end();
    if ((r_it = (*a_it)->rooms().find(vnum)) != r_end) {
      return r_it->second;
    }
  }
  return NULL;
}

/************************************************************ ITEMS ************************************************************/
void World::insert(Item* item) {
  getItems().insert(item);
  return;
}

void World::remove(Item* item) {
  getItems().erase(item);
  return;
}

Item* World::findItem(const std::string& name) {
  std::set<Item*>::iterator it;
  std::set<Item*>::iterator end = getItems().end();
  for (it = getItems().begin(); it != end; ++it) {
    if ((*it)->identifiers().matchesKeyword(name)) {
      return *it;
    }
  }
  return NULL;
}

bool World::transport(Item* item, Room* from, const unsigned long& vnum) {
  Room* to = NULL;

  if ((to = findRoom(vnum)) == NULL) {
    return false;
  }

  from->remove(item);
  from->send("$o disappears in small puff of smoke.\n", NULL, item);
  to->add(item);
  to->send("$o appears with a small \"pop!\"\n", NULL, item);
  return true;
}

/************************************************************ BOARDS ************************************************************/
void World::add(Board* board) {
  boards().insert(std::make_pair(board->number(), board));
  return;
}

void World::add(const unsigned short& board, Note* note) {
  boards()[board]->notes().insert(std::make_pair(note->ID(), note));
  return;
}

void World::saveBoards(void) {
  for (std::map<unsigned,Board*>::iterator it = boards().begin(); it != boards().end(); ++it) {
    it->second->save();
  }
  return;
}

/************************************************************ STATIC ************************************************************/
unsigned World::rand(const unsigned& min, const unsigned& max) {
  return (::rand() % (max-min+1) + min);
}

void World::worldLog(unsigned level, unsigned type, const char* format, ...) {
  char buffer[Socket::MAX_BUFFER];
  va_list args;

  // Process our arguments...
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  // Prep and send our query...
  fprintf(stderr, "LOG: %s\n", buffer);

  // Depending on the level (and the current system log level) we may also print the message...
  if (level == LOG_LEVEL_SYSTEM || level >= LOG_LEVEL_WARNING) {
    Instance().bigBrother(NULL, ADMIN_BIGBRO_SYSTEM, buffer);
    fprintf(stderr, "Log %s: %s\n", os::strnow().c_str(), buffer);
  }

  return;
}

void World::playerLog(unsigned level, unsigned type, const char* format, ...) {
  char buffer[Socket::MAX_BUFFER];
  va_list args;

  // Process our arguments...
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  // Prep and send our query...
  fprintf(stderr, "ERROR: %s\n", buffer);

  return;
}

bool World::search_map(Being* being, Room*** map, const unsigned short& ymax, const unsigned short& xmax, const short& y, const short& x, Room* room, std::string** display) {
  Exit* exit = NULL;
  short new_x = 0;
  short new_y = 0;
  short dis_x = 0;
  short dis_y = 0;
  std::string dis_s;
  // Bounds check...
  if (y < 0 || y > ymax*2 || x < 0 || x > xmax*2) {
    return true;
  }
  // There's a room at this location...
  if (map[y][x]) {
    // Collision check...
    if (map[y][x] != room) {
      return false;
    }
    // We've already been here; bail out...
    return true;
  }
  // Store our pointer and map position...
  map[y][x] = room;
  display[y*2][x*2] = get_marker(being, room);
  // Search from here...
  for (unsigned u = NORTH; u < UP; ++u) {
    new_x = x;
    new_y = y;
    switch (u) {
      case NORTH: new_y++;  dis_x = new_x*2;    dis_y = new_y*2-1;  dis_s = "|"; break;
      case EAST:  new_x++;  dis_x = new_x*2-1;  dis_y = new_y*2;    dis_s = "-"; break;
      case SOUTH: new_y--;  dis_x = new_x*2;    dis_y = new_y*2+1;  dis_s = "|"; break;
      case WEST:  new_x--;  dis_x = new_x*2+1;  dis_y = new_y*2;    dis_s = "-"; break;
      default: return false;
    }
    if ((exit = room->exit(u)) && new_y >= 0 && new_x >= 0) {
      if (exit->flags().test(EXIT_HIDDEN) || exit->targetRoom()->flags().test(ROOM_NOMAP)) {
        continue;
      }
      if (!World::search_map(being, map, ymax, xmax, new_y, new_x, room->exit(u)->targetRoom(), display)) {
        return false;
      }
      display[dis_y][dis_x] = dis_s;
    }
  }
  return true;
}

std::string World::get_marker(Being* being, Room* room) {
  char buf[8];
  if (being->room() == room) {
    return "{c@{x";
  } else {
    sprintf(buf, "{%c%c{x", room->terrain()->map(), '+');
    return buf;
  }
}
