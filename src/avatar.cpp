
#include <cstdarg>
#include <time.h>
#include "avatar.h"
#include "commandTable-default.h"
#include "creature.h"
#include <sodium/crypto_hash.h>
#include "estring.h"
#include "io-handler.h"
#include "object-container.h"
#include "storage.h"
#include "world.h"

const unsigned Avatar::DO_NOT_DELETE;
const unsigned Avatar::DELETE_ON_LOGIN;
const unsigned Avatar::DESTROY_NOW;

Avatar::Avatar(Socket* socket_) {
  time(&_loggedOn);
  status().set(CONNECTING);
  disconnected(false);
  deletionStatus(DO_NOT_DELETE);
  socket(socket_);
  if (socket() != nullptr) identifiers().shortname(estring(socket()->getFd()));
  board(0);
  note(NULL);
  gechoColor('x');
  roomNumber(0);
  room(NULL);
  mode().set(MODE_NONE);
  title(", adventurer.");
  age(17);
  bankSilver(0);
  bankGold(0);
  return;
}

Avatar::~Avatar(void) {
  delete socket();
  return;
}

/******************************************************* Avatar-specific methods ********************************************************/
/*
 * This methods exists basically to consolidate the underlying actions of
 * the various overloads of Avatar::send() so that we can do things like
 * color in a central location.
 */

// private
void Avatar::processOutput(std::string src) {
  output(output() + src);
  return;
}

// private
void Avatar::changeIdentifiers(std::string str) {
  identifiers().shortname(str);
  identifiers().getKeywords().clear();
  identifiers().getKeywords().insert(str);
  return;
}

bool Avatar::hasInput(void) {
  try {
    return socket()->hasInput();
  } catch (SocketException se) {
    World::Instance().bigBrother(this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::hasInput() -> %s", identifiers().shortname().c_str(), se.getError().c_str());
    disconnected(true);
    return false;
  }
}

std::string Avatar::getInput(void) {
  try {
    return socket()->getInput();
  } catch (SocketException se) {
    World::Instance().bigBrother(this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::getInput() -> %s", identifiers().shortname().c_str(), se.getError().c_str());
    disconnected(true);
    return std::string();
  }
}

bool Avatar::hasOutput(void) {
  try {
    return (output().length() > 0);
  } catch (SocketException se) {
    World::Instance().bigBrother(this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::hasOutput() -> %s", identifiers().shortname().c_str(), se.getError().c_str());
    disconnected(true);
    return false;
  }
}

void Avatar::flushOutput(void) {
  try {
    send(IOhandlers().back()->prompt());
    socket()->send(std::string("\n") + output().interpretColor());
    output(std::string());
  } catch (SocketException se) {
    World::Instance().bigBrother(this, ADMIN_BIGBRO_ERRORS, "%s threw a SocketException in Avatar::flushOutput() -> %s", identifiers().shortname().c_str(), se.getError().c_str());
    disconnected(true);
  }
  return;
}

std::string Avatar::listWhoFlags(void) {
  return whoFlags().list(FTAvatarWho::Instance(), true);
}

void Avatar::restoreRoom(void) {
  CmdLook look;
  std::vector<std::string> look_args(1);

  // put the Avatar in the right place...
  room(World::Instance().findRoom(roomNumber()));
  if (!room()) { // just in case
    room(World::Instance().findRoom(0));
  }
  room()->add(this);
  // let everyone know...
  room()->send_cond("$p has entered the realm.\n", this);
  // show them around...
  look.avatar(this);
  look.execute(this, look_args);
  return;
}

std::string Avatar::stringLoggedOn(void) {
  return World::realtime(secondsLoggedOn());
}

/****************************** Persistence ******************************/
void Avatar::save(void) {
  FILE* fp = nullptr;
  const char* filename = Storage::filename(this).c_str();

  if ((fp = fopen(filename, "w")) != NULL) {
    Storage::dump(fp, this);
    fclose(fp);
  } else {
    fprintf(stderr, "Failed to write avatar file %s.\n", filename);
  }

  return;
}

bool Avatar::load(void) {
  const char* filename = Storage::filename(this).c_str();
  FILE* fp = nullptr;

  if ((fp = fopen(filename, "r")) != NULL) {
    Storage::load(fp, this);
    fclose(fp);
    return true;
  } else {
    fprintf(stderr, "Failed to read avatar file %s.\n", filename);
    return false;
  }
}

// changeName is used for internal procedural purposes
void Avatar::changeName(std::string name) {
  World::Instance().remove(this);
  changeIdentifiers(name);
  World::Instance().insert(this);
  return;
}

// rename is used to permanently modify the name of a player
bool Avatar::rename(std::string new_name) {
  std::string old_name = name();

  changeIdentifiers(new_name);
  std::string new_filename = Storage::filename(this);

  if (Storage::file_exists(new_filename)) {
    // Already taken
    changeIdentifiers(old_name);
    return false;
  } else {
    // Good to go; there's a bit of a song and dance because of how World::insert()
    // and World::remove() operate on both the Creature and Avatar containers.
    changeIdentifiers(old_name);
    World::Instance().remove(this);
    changeIdentifiers(new_name);
    World::Instance().insert(this);
    // TODO - delete the old file
    save();
    return true;
  }

  return false;
}

std::string Avatar::hashPassword(std::string source) {
  size_t length = source.size();
  unsigned char* str = new unsigned char[length];
  unsigned char hash[crypto_hash_BYTES];
  char buf[3];
  std::string dest;

  // Copy the source string into the buffer
  for (int x = 0; x < length; ++x) str[x] = source[x];
  str[length] = 0;

  // libsodium gives us the SHA512
  crypto_hash(hash, str, length);

  // convert the raw hash buffer to printable hex
  for (size_t b = 0; b < crypto_hash_BYTES; ++b) {
    sprintf(buf, "%02X", hash[b]);
    dest += buf;
  }

  delete str;
  return dest;
}

void Avatar::setPassword(std::string str) {
  password(hashPassword(str));
  return;
}

bool Avatar::checkPassword(std::string attempt) {
  return hashPassword(attempt) == password();
}

void Avatar::destroy(void) {
  std::string original_path = Storage::filename(this);
  std::string original_base = basename(original_path.c_str());
  std::string deadpool_path = Storage::deadpool_directory();
  std::string deadpool_base = original_base;

  // Append a timestamp to the filename.
  std::string now = World::strnow();
  Regex::replace("[^0-9]+", "-", now);
  deadpool_base += "_time_" + now;

  // Append the IP address to the filename.
  std::string ip = socket()->getIP();
  Regex::replace("[^0-9]+", "-", ip);
  deadpool_base += "_ip_" + ip;

  // Move the avatar data file to the deadpool.
  ::rename(original_path.c_str(), (deadpool_path + deadpool_base).c_str());

  delete this;
  return;
}

bool Avatar::shouldDelete(void) const {
  return _deletionStatus == DELETE_ON_LOGIN;
}

bool Avatar::shouldDestroy(void) const {
  return _deletionStatus == DESTROY_NOW;
}

/******************************************************* Overloads of virtual methods ********************************************************/
void Avatar::send(std::string message) {
  processOutput(message);
  return;
}

void Avatar::send(const char* format, ...) {
  char buffer[Socket::MAX_BUFFER];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  processOutput(buffer);
  return;
}

void Avatar::room(Room* room) {
  CmdExit exit;
  std::vector<std::string> exit_args(1);
  exit.avatar(this);

  Creature::room(room);

  if (mode().number() == MODE_REDIT) {
    if (!this->room()->area()->hasPermission(this)) {
      while (mode().number() != MODE_NONE) {
        exit.execute(this, exit_args);
      }
      return;
    }
    // Make sure no one else is editing the room...
    for (auto iter : World::Instance().getAvatars()) {
      if (iter.second->isConnected() && iter.second != this && iter.second->mode().number() == MODE_REDIT && iter.second->room() == room) {
        send("Sorry, %s is already editing this room.", seeName(iter.second).c_str());
        while (mode().number() != MODE_NONE) {
          exit.execute(this, exit_args);
        }
        return;
      }
    }
  }

  return;
}

void Avatar::title(std::string title) {
  std::string foo = Regex::trim(title);

  _title.clear();

  if (foo.empty()) {
    return;
  }

  switch (foo[0]) {
    case ',':
    case '.':
    case ':':
    case ';':
      break;
    default:
      _title.assign(1, ' ');
  }

  _title.append(foo);

  return;
}

void Avatar::whatHappensWhenIDie(void) {
  CmdLook look;
  std::vector<std::string> look_args(1);
  // Leave the current Room.
  room()->remove(this);
  // Relocate to the spawn point.
  room(World::Instance().findRoom(0));
  room()->add(this);
  // It's alive! It's allllliiiiiiiiiiiiive!
  action().set(0);
  // let everyone know...
  room()->send_cond("$p appears, looking weary.\n", this);
  // show them around...
  look.avatar(this);
  look.execute(this, look_args);
  return;
}
