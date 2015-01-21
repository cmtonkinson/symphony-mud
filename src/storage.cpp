
#include <cstring>
#include "storage.h"

const unsigned Storage::LOAD_DONE;
const unsigned Storage::LOAD_NULL;
const unsigned Storage::LOAD_NEW;

void Storage::dump(FILE* fp, Exit* exit) {
  const char* token = "EXIT";
  BEGIN(token)
  out(fp, "direction",  exit->direction());
  out(fp, "key",        exit->key());
  END(token)
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
    next = fgetc(fp);
    fseek(fp, -1, SEEK_CUR);
    // If it's a slash, we're done loading this object.
    if (next == '/') return LOAD_DONE;
    // If the next character is capitalized, switch modes.
    if (std::isupper(next)) return LOAD_NEW;
    // The next input looks like a key - keep iterating.
  }

  return LOAD_DONE;
}
