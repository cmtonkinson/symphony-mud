
#include <glob.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include "avatar.hpp"
#include "being.hpp"
#include "command.hpp"
#include "npc.hpp"
#include "os.hpp"
#include "regex.hpp"
#include "world.hpp"
#include "zone.hpp"

#ifdef __APPLE__
#include <libgen.h>
#endif

namespace os {

  /////////////////////////////////////////////////////////////////////////////
  // TIME
  /////////////////////////////////////////////////////////////////////////////
  unsigned long now(void) {
    return time(NULL);
  }

  std::string strnow(void) {
    char buffer[BUFFER_SIZE];
    time_t seconds = time(NULL);
    tm* info = localtime(&seconds);
    strftime(buffer, BUFFER_SIZE, "%Y/%m/%d %H:%M:%S", info);
    return buffer;
  }

  std::string strtime(time_t in_time) {
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

  std::string realtime(const unsigned long& seconds, unsigned granularity) {
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

  /////////////////////////////////////////////////////////////////////////////
  // LOGGING
  /////////////////////////////////////////////////////////////////////////////
  unsigned console_log_level = 0;

  void log(const char* func, const char* file, unsigned line, unsigned level, const Being* being, const char* format) {
    log_(func, file, line, level, being, format);
    return;
  }

  void log_(const char* func, const char* file, unsigned line, unsigned level, const Being* being, const char* format, ...) {
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
    message << strnow() << " ";
    // Function/file/line
    message << "[" << func << "() " << file << ":" << line << "] ";
    // Being details (if given)
    if (being) message << "(" << being->ident() << ") ";
    // Content
    message << buffer << "\n";

    // Conditionally output to console
    if (level >= console_log_level) {
      if (level <= LOG_INFO) std::cout << message;
      else std::cerr << message;
    }

    // Broadcast to admins
    World::Instance().bigBrother(being, level, message);

    return;
  }

  const char* stringLevel(unsigned level) {
    switch (level) {
      case LOG_SILLY:   return "SILLY";
      case LOG_DEBUG:   return "DEBUG";
      case LOG_VERBOSE: return "VERBOSE";
      case LOG_INFO:    return "INFO";
      case LOG_WARN:    return "WARN";
      case LOG_ERROR:   return "ERROR";
      case LOG_FATAL:   return "FATAL";
      case LOG_SILENT:  return "SILENT";
      default:          return "UNKNOWN";
    }
  }

  unsigned numericLevel(std::string level) {
    if      (Regex::strPrefix(level, "silly")) return LOG_SILLY;
    else if (Regex::strPrefix(level, "debug")) return LOG_DEBUG;
    else if (Regex::strPrefix(level, "verbose")) return LOG_VERBOSE;
    else if (Regex::strPrefix(level, "info")) return LOG_INFO;
    else if (Regex::strPrefix(level, "warn")) return LOG_WARN;
    else if (Regex::strPrefix(level, "error")) return LOG_ERROR;
    else if (Regex::strPrefix(level, "fatal")) return LOG_FATAL;
    else if (Regex::strPrefix(level, "silent")) return LOG_SILENT;
    else return -1;
  }

  std::string allLevels(void) {
    std::vector<std::string> dest;
    for (int lvl = LOG_SILLY; lvl <= LOG_SILENT; ++lvl) dest.push_back(stringLevel(lvl));
    return Regex::implode(", ", dest);
  }

  char color(unsigned level) {
    switch (level) {
      case LOG_SILLY:   return 'w';
      case LOG_DEBUG:   return 'w';
      case LOG_VERBOSE: return 'b';
      case LOG_INFO:    return 'B';
      case LOG_WARN:    return 'Y';
      case LOG_ERROR:   return 'R';
      case LOG_FATAL:   return 'R';
      default:          return 'x';
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // FILESYSTEM PATHS
  /////////////////////////////////////////////////////////////////////////////
  std::string avatar_glob_pattern(void) {
    return "data/avatars/*.avatar.txt";
  }

  std::string zone_glob_pattern(void) {
    return "data/zones/*.zone.txt";
  }

  std::string social_glob_pattern(void) {
    return "data/socials/*.social.txt";
  }

  std::string disabled_command_glob_pattern(void) {
    return "data/disabled_commands/*_*";
  }

  std::vector<std::string> glob(std::string pattern) {
    std::vector<std::string> paths;
    glob_t globbuf;
    int return_status = 0;

    return_status = ::glob(pattern.c_str(), GLOB_NOSORT, NULL, &globbuf);

    switch (return_status) {
      case 0: // success
        for (size_t x = 0; x < globbuf.gl_pathc; ++x) paths.push_back(globbuf.gl_pathv[x]);
        break;
      case GLOB_NOSPACE:
        ERROR_(0, "out of memory on %s", pattern.c_str())
        break;
      case GLOB_NOMATCH:
        DEBUG_(0, "no matches on %s", pattern.c_str())
        break;
      case GLOB_ABORTED:
        ERROR_(0, "read error on %s", pattern.c_str())
        break;
    }

    globfree(&globbuf);
    return paths;
  }

  std::string filename(Avatar* avatar) {
    return std::string("data/avatars/") + Regex::slugify(avatar->name()) + ".avatar.txt";
  }

  std::string filename(Zone* zone) {
    return std::string("data/zones/") + Regex::slugify(zone->name()) + ".zone.txt";
  }

  std::string filename(SocialCommand* social) {
    return std::string("data/socials/") + Regex::slugify(social->name()) + ".social.txt";
  }

  std::string deadpool_directory(void) {
    return "data/deadpool/";
  }

  std::string basename(const std::string path) {
    #ifdef __APPLE__
    char* p = nullptr;
    strcpy(p, path.c_str());
    return ::basename(p);
    #else
    return ::basename(path.c_str());
    #endif
  }

  bool file_exists(std::string path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
  }

};
