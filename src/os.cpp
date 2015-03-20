
#include <glob.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include "avatar.h"
#include "being.h"
#include "command.h"
#include "githash.h"
#include "npc.h"
#include "os.h"
#include "zone.h"

///////////////////////////////////////////////////////////////////////////////
// TIME
///////////////////////////////////////////////////////////////////////////////
unsigned long os::now(void) {
  return time(NULL);
}

std::string os::strnow(void) {
  char buffer[BUFFER_SIZE];
  time_t seconds = time(NULL);
  tm* info = localtime(&seconds);
  strftime(buffer, BUFFER_SIZE, "%Y/%m/%d %H:%M:%S", info);
  return buffer;
}

std::string os::strtime(time_t in_time) {
  char buffer[BUFFER_SIZE];
  time_t seconds = 0;
  if (in_time) {
    seconds = in_time;
  } else {
    seconds = time(NULL);
  }
  tm* info = localtime(&seconds);
  strftime(buffer, BUFFER_SIZE, "%A, %B %d, %Y  %H:%M:%S", info);
  return buffer;
}

std::string os::realtime(const unsigned long& seconds, unsigned granularity) {
  char buffer[BUFFER_SIZE];
  char foo[100];
  unsigned long sec = seconds;
  unsigned long min = 0;
  unsigned long hour = 0;
  unsigned long day = 0;
  unsigned long year = 0;

  memset(buffer, 0, BUFFER_SIZE);

  while (sec >= 60) {
    ++min;
    sec -= 60;
  }

  if (granularity > GRAN_MINUTE) {
    while (min >= 60) {
      ++hour;
      min -= 60;
    }
    if (granularity > GRAN_HOUR) {
      while (hour >= 24) {
        ++day;
        hour -= 24;
      }
      if (granularity > GRAN_DAY) {
        while (day >= 365) {
          ++year;
          day -= 365;
        }
      }
    }
  }

  if (year) {
    if (year == 1) {
      sprintf(foo, "%lu year", year);
    } else {
      sprintf(foo, "%lu years", year);
    }
    strcat(buffer, foo);
  }

  if (day) {
    if (day == 1) {
      sprintf(foo, "%lu day", day);
    } else {
      sprintf(foo, "%lu days", day);
    }
    if (year) {
      strcat(buffer, ", ");
    }
    strcat(buffer, foo);
  }

  if (hour) {
    if (hour == 1) {
      sprintf(foo, "%lu hour", hour);
    } else {
      sprintf(foo, "%lu hours", hour);
    }
    if (year || day) {
      strcat(buffer, ", ");
    }
    strcat(buffer, foo);
  }

  if (min) {
    if (min == 1) {
      sprintf(foo, "%lu minute", min);
    } else {
      sprintf(foo, "%lu minutes", min);
    }
    if (year || day || hour) {
      strcat(buffer, ", ");
    }
    strcat(buffer, foo);
  }

  if (sec) {
    if (sec == 1) {
      sprintf(foo, "%lu second", sec);
    } else {
      sprintf(foo, "%lu seconds", sec);
    }
    if (year || day || hour || min) {
      strcat(buffer, ", ");
    }
    strcat(buffer, foo);
  }

  return buffer;
}

///////////////////////////////////////////////////////////////////////////////
// LOGGING
///////////////////////////////////////////////////////////////////////////////
void os::log(const char* file, unsigned line, unsigned level, Being* being, const char* format) {
  os::log_(file, line, level, being, format);
  return;
}

void os::log_(const char* file, unsigned line, unsigned level, Being* being, const char* format, ...) {
  char buffer[BUFFER_SIZE];
  std::string message;
  va_list args;

  if (level == LOG_SILENT) return; // don't even bother

  // Print variable arguments to the buffer.
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  // Log level
  message << stringLevel(level) << " ";
  // Timestamp
  message << os::strnow() << " ";
  // File/line
  message << "[" << file << ":" << line << " " << GIT_HASH << "] ";
  // Being details (if given)
  if (being) message << "(" << being->ident() << ") ";
  // Content
  message << buffer << "\n";

  std::cout << message; // TODO

  return;
}

const char* os::stringLevel(unsigned level) {
  switch (level) {
    case LOG_SILENT:  return "SILENT";
    case LOG_FATAL:   return "FATAL";
    case LOG_ERROR:   return "ERROR";
    case LOG_WARN:    return "WARN";
    case LOG_INFO:    return "INFO";
    case LOG_VERBOSE: return "VERBOSE";
    case LOG_DEBUG:   return "DEBUG";
    case LOG_SILLY:   return "SILLY";
    default:          return "UNKNOWN";
  }
}

///////////////////////////////////////////////////////////////////////////////
// FILESYSTEM PATHS
///////////////////////////////////////////////////////////////////////////////
std::string os::avatar_glob_pattern(void) {
  return "data/avatars/*.avatar.txt";
}

std::string os::zone_glob_pattern(void) {
  return "data/zones/*.zone.txt";
}

std::string os::social_glob_pattern(void) {
  return "data/socials/*.social.txt";
}

std::string os::disabled_command_glob_pattern(void) {
  return "data/disabled_commands/*_*";
}

std::vector<std::string> os::glob(std::string pattern) {
  std::vector<std::string> paths;
  glob_t globbuf;
  int return_status = 0;

  return_status = ::glob(pattern.c_str(), GLOB_NOSORT, NULL, &globbuf);

  switch (return_status) {
    case 0: // success
      for (size_t x = 0; x < globbuf.gl_pathc; ++x) paths.push_back(globbuf.gl_pathv[x]);
      break;
    case GLOB_NOSPACE:
      ERROR_(0, "glob - out of memory on %s", pattern.c_str())
      break;
    case GLOB_NOMATCH:
      DEBUG_(0, "glob - no matches on %s", pattern.c_str())
      break;
    case GLOB_ABORTED:
      ERROR_(0, "glob - read error on %s", pattern.c_str())
      break;
  }

  globfree(&globbuf);
  return paths;
}

std::string os::filename(Avatar* avatar) {
  return std::string("data/avatars/") + Regex::slugify(avatar->name()) + ".avatar.txt";
}

std::string os::filename(Zone* zone) {
  return std::string("data/zones/") + Regex::slugify(zone->name()) + ".zone.txt";
}

std::string os::filename(SocialCommand* social) {
  return std::string("data/socials/") + Regex::slugify(social->name()) + ".social.txt";
}

std::string os::deadpool_directory(void) {
  return "data/deadpool/";
}

bool os::file_exists(std::string path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}
