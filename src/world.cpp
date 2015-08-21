
#include <cstdarg>
#include <dirent.h>
#include "command-def.hpp"
#include "os.hpp"
#include "storage.hpp"
#include "world.hpp"

World::World(void) {
  // basic setup
  booted(time(NULL));
  exists(true);
  rebooting(false);
  jobsPerTurn(10);
  tickSleep(500);
  schedule()->add(new RecurringJob(this, &World::tock, "World::tock", 50, 70));
  schedule()->add(new RecurringJob(this, &World::save, "World::save", 900));
  return;
}

World::~World(void) {
  return;
}

/************************************************************ CONTROL ************************************************************/
void World::startup(void) {
  load();
  npcIOHandler()->commandTable(&(Commands::Instance()));
  exist();
  return;
}

void World::exist(const unsigned int& fd) {
  try {
    if (rebooting()) {
      getServer()->setFd(fd);
    } else {
      getServer()->startup(6501);
    }
  } catch (SocketException se) {
    ERROR_(0, "can't start Socket server: %s", se.getError().c_str())
    exit(os::EXIT_FAILED_BOOT);
  }

  if (rebooting())  INFO_(0, "system reboot on port %u", getServer()->getPort())
  else              INFO_(0, "system boot on port %u", getServer()->getPort())

  while (exists()) tick();

  if (!save()) ERROR(0, "failed to save")

  INFO(0, "system shutdown")
  return;
}

void World::tick(void) {
  handleNewConnections();
  handleJobs();
  handleInput();
  handleOutput();
  handleDisconnects();
  usleep(tickSleep()); // CPU throttle
  return;
}

bool World::reboot(Being* being) {
  std::map<std::string,Avatar*>::iterator it;
  estring socket;
  estring verbosity;
  FILE* fp = NULL;

  save();

  fp = fopen(os::REBOOT_FILE, "w");
  if (!fp) {
    if (being) being->send("Copyover file couldn't be opened for writing! Copyover aborted!");
    ERROR(being, "failed to write copyover file")
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
    INFO(0, "system saved for reboot")
  } else {
    ERROR(0, "failed to save; copyover aborted")
    return false;
  }

  // Pass the current server socket fd to the child.
  socket = estring("-s ").append(estring(getServer()->getFd()));
  // Preserve the current verbosity settings.
  verbosity = estring("-").append(estring(os::console_log_level));
  // Make the system call.
  DEBUG_(0, "executing reboot with '%s %s'", socket.c_str(), verbosity.c_str())
  if (execl("bin/symphony", "symphony", socket.c_str(), verbosity.c_str(), NULL) < 0) ERROR(0, "reboot failed")

  return false;
}

void World::recover(const unsigned int& fd) {
  FILE* fp = NULL;
  int client_fd = 0;
  char ip[Socket::MAX_BUFFER];
  char name[Socket::MAX_BUFFER];
  Avatar* avatar = NULL;

  if (!load()) ERROR(0, "failed to recover")

  fp = fopen(os::REBOOT_FILE, "r");

  if (!fp) {
    ERROR(0, "failed to read copyover file")
    exit(os::EXIT_FAILED_REBOOT);
  }

  while (true) {
    if (fscanf(fp, "%d %s %s", &client_fd, ip, name) < 3) break;
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
  unlink(os::REBOOT_FILE);

  INFO(0, "system reboot")
  broadcast("\n\nCopyover successful. You may now drop your pants.");

  exist(fd);

  return;
}

bool World::load(void) {
  bool status = true;

  loadZones();
  loadDisabledCommands();
  loadSocials();

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

  INFO(0, "system save")
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

bool World::tock(RecurringJob* job) {
  VERBOSE(0, "tock")
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
      ERROR_(0, "SocketException: %s", se.getError().c_str())
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
      ERROR_(0, "SocketException: %s", se.getError().c_str())
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
    new_connections.erase(sock_it);
    INFO_(0, "new connection from %s", (*sock_it)->getIP().c_str())
  }
  return;
}

void World::handleDisconnects(void) {
  std::map<std::string,Avatar*>::iterator next;
  for (std::map<std::string,Avatar*>::iterator a_it = getAvatars().begin(); a_it != getAvatars().end();) {
    if (a_it->second->disconnected()) {
      // We have to go through a bit of a song and dance here, because map::erase() does
      // not return the next valid iterator in the sequence like vector::erase() does.
      next = a_it;
      ++next;
      INFO(a_it->second, "log out")
      if (a_it->second->room()) {
        a_it->second->room()->send_cond("$a has left the realm.", a_it->second);
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
      it->second->send(message + "\n");
    }
  }
  return;
}

void World::bigBrother(const Being* being, unsigned level, std::string message) {
  Avatar* avatar = nullptr;
  std::string munged = message;
  std::string line_break;

  // Prettify
  line_break << "\n{" << os::color(level);
  Regex::replace("(?<=\\]) ", line_break, munged);

  for (auto iter : getAvatars()) {
    avatar = iter.second;
    if (!avatar->isConnected()) continue;
    if (!avatar->immortal()) continue;
    if (avatar == being) continue;
    if (avatar->bigBrother() > level) continue;
    avatar->send("BigBrother: %s{x", munged.c_str());
  }
  return;
}

/************************************************************ JOBS ************************************************************/
void World::handleJobs(void) {
  unsigned count = 0;
  while (schedule()->fire()) {
    if (++count >= jobsPerTurn()) break;
  }
  return;
}

/************************************************************ BEINGS ************************************************************/
void World::insert(Being* being) {
  getBeings().insert(being);
  if (being->isAvatar()) {
    getAvatars().insert(std::make_pair(being->name(), dynamic_cast<Avatar*>(being)));
  }
  return;
}

void World::remove(Being* being) {
  getBeings().erase(being);
  if (being->isAvatar()) {
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

unsigned long World::nextZoneID(void) const {
  unsigned long nextID = 0;
  for (auto iter : getZones()) {
    if (iter->ID() > nextID) {
      nextID = iter->ID();
    }
  }
  return nextID + 1;
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
