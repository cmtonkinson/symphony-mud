
#include "exit.hpp"
#include "regex.hpp"
#include "room.hpp"
#include "world.hpp"

Exit::Exit(void) {
  _targetVnum = 0;
  _targetRoom = nullptr;
  return;
}

Exit::Exit(Room* room, Room* targetRoom_, unsigned direction_) {
  targetRoom(targetRoom_);
  targetVnum(targetRoom()->vnum());
  direction().set(direction_);
  flags().value(0);
  key(0);
  return;
}

Exit::~Exit(void) {
  return;
}

void Exit::destroy(void) {
  delete this;
  return;
}

/*
 * Note: This method is not designed for efficiency. For optimal runtime computational efficiency
 * Rooms and Exits would be loaded in two passes, and Exits would simply retain pointers to their
 * targets (obviating the need to store derived data - the vnum - separately in memory).
 *
 * But because of how Zones are loaded (specifically, how Exits for a Room are nested within the
 * serialization of the Room itself, this isn't feasible and a single-pass loading paradigm makes
 * more sense from a code maintainability standpoint. So instead we load the vnums from the data
 * files and lazy-load the target pointers.
 */
Room* Exit::targetRoom(void) {
  // Is the target Room cached?
  if (_targetRoom != nullptr) return _targetRoom;
  // Find, cache, and return the target Room.
  _targetRoom = World::Instance().findRoom(targetVnum());
  if (_targetRoom == nullptr) fprintf(stderr, "Failed to find Room %u in Exit::targetRoom().\n", targetVnum());
  return _targetRoom;
}

void Exit::flag(const unsigned long& flag, const bool& value, bool stop) {
  Exit* other = NULL;
  // Set the exit flag...
  _flags.set(flag, value);
  // If we find an opposite exit, also change that flag...
  if (!stop) { // this prevents recursion
    if ((other = targetRoom()->exit(inverse(direction().number()))) != NULL) {
      other->flag(flag, value, true);
    }
  }
  return;
}

unsigned Exit::inverse(unsigned direction) { // static public
  switch (direction) {
    case NORTH: return SOUTH;
    case EAST:  return WEST;
    case SOUTH: return NORTH;
    case WEST:  return EAST;
    case UP:    return DOWN;
    case DOWN:  return UP;
    default:    return UNDEFINED;
  }
}

const char* Exit::name(unsigned direction) { // static public
  switch (direction) {
    case NORTH: return "North";
    case EAST:  return "East";
    case SOUTH: return "South";
    case WEST:  return "West";
    case UP:    return "Up";
    case DOWN:  return "Down";
    default:    return "error";
  }
}

const char* Exit::inverseName(unsigned direction) { // static public
  switch (direction) {
    case NORTH: return "South";
    case EAST:  return "West";
    case SOUTH: return "North";
    case WEST:  return "East";
    case UP:    return "Down";
    case DOWN:  return "Up";
    default:    return "error";
  }
}

unsigned Exit::string2dir(const std::string& str) { // static public
  std::string dir = Regex::trim(str);
  if (Regex::strPrefix(str, "north")) {
    return NORTH;
  } else if (Regex::strPrefix(str, "east")) {
    return EAST;
  } else if (Regex::strPrefix(str, "south")) {
    return SOUTH;
  } else if (Regex::strPrefix(str, "west")) {
    return WEST;
  } else if (Regex::strPrefix(str, "up")) {
    return UP;
  } else if (Regex::strPrefix(str, "down")) {
    return DOWN;
  } else {
    return UNDEFINED;
  }
}

unsigned Exit::string2inverse(const std::string& str) { // static public
  std::string dir = Regex::trim(str);
  if (Regex::strPrefix(str, "north")) {
    return SOUTH;
  } else if (Regex::strPrefix(str, "east")) {
    return WEST;
  } else if (Regex::strPrefix(str, "south")) {
    return NORTH;
  } else if (Regex::strPrefix(str, "west")) {
    return EAST;
  } else if (Regex::strPrefix(str, "up")) {
    return DOWN;
  } else if (Regex::strPrefix(str, "down")) {
    return UP;
  } else {
    return UNDEFINED;
  }
}
