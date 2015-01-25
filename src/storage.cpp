
#include <cstring>
#include "exit.h"
#include "room.h"
#include "storage.h"

const unsigned Storage::LOAD_DONE;
const unsigned Storage::LOAD_NULL;
const unsigned Storage::LOAD_NEW;

/***************************************************************************************************
 * ROOM
 **************************************************************************************************/
void Storage::dump(FILE* fp, Room* room) {
  BEGIN("ROOM")
  out(fp, "vnum",         room->vnum());
  out(fp, "name",         room->name());
  out(fp, "description",  room->description());
  out(fp, "smell",        room->smell());
  out(fp, "sound",        room->sound());
  out(fp, "terrain",      room->terrain()->name());
  for (unsigned u = 0; u < 6; ++u) {
    if (room->exit(u)) dump(fp, room->exit(u));
  }
  END("ROOM")
  return;
}

bool Storage::load(FILE* fp, Room* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "ROOM", [&fp, &loading, &input]() {
    STORE_CASE("vnum",        &Room::vnum)
    STORE_CASE("name",        &Room::name)
    STORE_CASE("description", &Room::description)
    STORE_CASE("smell",       &Room::smell)
    STORE_CASE("sound",       &Room::sound)
    STORE_CASE("terrain",     &Room::setTerrain)
    STORE_DESCEND("EXIT", Exit,
      loading->exit(instance->direction().number(), instance);
    )
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * EXIT
 **************************************************************************************************/
void Storage::dump(FILE* fp, Exit* exit) {
  BEGIN("EXIT")
  out(fp, "direction",  exit->direction());
  out(fp, "key",        exit->key());
  END("EXIT")
  return;
}

bool Storage::load(FILE* fp, Exit* loading) {
  char input[32];
  unsigned load_status = 0;
  load_status = load_inner(fp, loading, input, "EXIT", [&fp, &loading, &input]() {
    STORE_CASE("direction", &Exit::direction)
    STORE_CASE("key",       &Exit::key)
  });
  return load_status == LOAD_DONE;
}

/***************************************************************************************************
 * INTERNAL METHODS
 **************************************************************************************************/

unsigned Storage::load_inner(FILE* fp, void* loading, char* input, const char* boundary, voidFunc lambda) {
  int next = 0;
  char end_boundary[32];
  sprintf(end_boundary, "/%s", boundary);

  // The first token should be the token for the object.
  fscanf(fp, "%s", input);
  if (feof(fp) || strcmp(input, boundary) != 0) return LOAD_NULL;

  // Iteratively look for new keys.
  while (!feof(fp)) {
    // Get the input.
    fscanf(fp, "%s", input);
    // Stop if this is the end token.
    if (strcmp(input, end_boundary) == 0) break;
    // Look for valid keys.
    lambda();
    // Test the stream (and reset the pointer).
    next = peek(fp)[0];
    // If the next character is capitalized, switch modes.
    if (std::isupper(next)) return LOAD_NEW;
    // The next input looks like a key - keep iterating.
  }
  return LOAD_DONE;
}

std::string Storage::read_string(FILE* fp) {
  std::string output;
  unsigned length = 0;
  char* buffer    = nullptr;
  // Find the length of the string and allocate a buffer.
  fscanf(fp, "%zu:", &length);
  if (length < 1) return ""; // Empty string
  buffer = new char[length + 1];
  // Read the string and terminate with null.
  fgets(buffer, length + 1, fp);
  buffer[length] = 0;
  // Convert the data to a string and clean up.
  output = buffer;
  delete buffer;
  return output;
}

std::string Storage::peek(FILE* fp) {
  char buf[32];
  fgets(buf, 32, fp);
  fseek(fp, -strlen(buf), SEEK_CUR);
  return buf;
}

void Storage::out(FILE* fp, const char* key, const char* value) {
  fprintf(fp, "%s %u:%s\n", key, strlen(value), value);
  return;
}

template <class ObjectType>
void Storage::in(FILE* fp, ObjectType* object, void (ObjectType::*method)(const char*)) {
  std::string buf = read_string(fp);
  (*object.*method)(buf.c_str());
  return;
}

template <class ObjectType>
void Storage::in(FILE* fp, ObjectType* object, void (ObjectType::*method)(std::string)) {
  std::string buf = read_string(fp);
  (*object.*method)(buf);
  return;
}
