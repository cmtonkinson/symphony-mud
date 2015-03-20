
#include <iostream>
#include <cstdio>
#include "githash.h"
#include "log.h"
#include "world.h"

const unsigned Log::LEVEL_SILENT;
const unsigned Log::LEVEL_ERROR;
const unsigned Log::LEVEL_WARN;
const unsigned Log::LEVEL_INFO;
const unsigned Log::LEVEL_VERBOSE;
const unsigned Log::LEVEL_SILLY;

void Log::log(const char* file, unsigned line, unsigned level, const char* format, ...) {
  char buffer[BUFFER_SIZE];
  char message[BUFFER_SIZE * 2];
  va_list args;

  if (level == LEVEL_SILENT) return; // don't even bother

  // Print variable arguments to the buffer.
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  // Format the final output message.
  sprintf(message, "%s %s [%s:%u %s] %s\n",
    stringLevel(level),
    World::strnow().c_str(),
    file,
    line,
    GIT_HASH,
    buffer
  );

  // stderr/stdout
  if (level >= LEVEL_WARN) {
    std::cerr << message;
  } else if (level <= LEVEL_INFO) {
    std::cout << message;
  }

  return;
}

const char* Log::stringLevel(unsigned level) {
  switch (level) {
    case LEVEL_SILENT:  return "SILENT";
    case LEVEL_ERROR:   return "ERROR";
    case LEVEL_WARN:    return "WARN";
    case LEVEL_INFO:    return "INFO";
    case LEVEL_VERBOSE: return "VERBOSE";
    case LEVEL_SILLY:   return "SILLY";
    default:            return "UNKNOWN";
  }
}
