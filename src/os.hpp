
#ifndef H_SYMPHONY_OS
#define H_SYMPHONY_OS

#include <string>
#include <vector>

class Avatar;
class Being;
class SocialCommand;
class Zone;

namespace os {

  const unsigned BUFFER_SIZE = 256;

  /////////////////////////////////////////////////////////////////////////////
  // TIME
  /////////////////////////////////////////////////////////////////////////////
  const unsigned GRAN_MINUTE = 0;
  const unsigned GRAN_HOUR   = 1;
  const unsigned GRAN_DAY    = 2;
  const unsigned GRAN_YEAR   = 3;
  const unsigned MINUTE      = 60;
  const unsigned HOUR        = MINUTE*60;
  const unsigned DAY         = HOUR*24;
  const unsigned YEAR        = DAY*365;

  unsigned long   now(void);
  std::string     strnow(void);
  std::string     strtime(time_t t = 0);
  std::string     realtime(const unsigned long& seconds, unsigned granularity = GRAN_DAY);

  /////////////////////////////////////////////////////////////////////////////
  // LOGGING
  /////////////////////////////////////////////////////////////////////////////
  const unsigned LOG_SILLY   = 1;
  const unsigned LOG_DEBUG   = 2;
  const unsigned LOG_VERBOSE = 3;
  const unsigned LOG_INFO    = 4;
  const unsigned LOG_WARN    = 5;
  const unsigned LOG_ERROR   = 6;
  const unsigned LOG_FATAL   = 7;
  const unsigned LOG_SILENT  = 8;

  extern unsigned console_log_level;

  void          log(const char* file, unsigned line, unsigned level, const Being* being, const char* format);
  void          log_(const char* file, unsigned line, unsigned level, const Being* being, const char* format, ...);
  const char*   stringLevel(unsigned level);
  unsigned      numericLevel(std::string level);
  std::string   allLevels(void);
  char          color(unsigned level);

  // Remember, #defines within a namespace will still bind to global scope.
  #define SILLY(being, format)          os::log(__FILE__, __LINE__, os::LOG_SILLY, being, format);
  #define SILLY_(being, format, ...)    os::log_(__FILE__, __LINE__, os::LOG_SILLY, being, format, __VA_ARGS__);

  #define DEBUG(being, format)           os::log(__FILE__, __LINE__, os::LOG_DEBUG, being, format);
  #define DEBUG_(being, format, ...)     os::log_(__FILE__, __LINE__, os::LOG_DEBUG, being, format, __VA_ARGS__);

  #define VERBOSE(being, format)        os::log(__FILE__, __LINE__, os::LOG_VERBOSE, being, format);
  #define VERBOSE_(being, format, ...)  os::log_(__FILE__, __LINE__, os::LOG_VERBOSE, being, format, __VA_ARGS__);

  #define INFO(being, format)           os::log(__FILE__, __LINE__, os::LOG_INFO, being, format);
  #define INFO_(being, format, ...)     os::log_(__FILE__, __LINE__, os::LOG_INFO, being, format, __VA_ARGS__);

  #define WARN(being, format)           os::log(__FILE__, __LINE__, os::LOG_WARN, being, format);
  #define WARN_(being, format, ...)     os::log_(__FILE__, __LINE__, os::LOG_WARN, being, format, __VA_ARGS__);

  #define ERROR(being, format)          os::log(__FILE__, __LINE__, os::LOG_ERROR, being, format);
  #define ERROR_(being, format, ...)    os::log_(__FILE__, __LINE__, os::LOG_ERROR, being, format, __VA_ARGS__);

  #define FATAL(being, format)          os::log(__FILE__, __LINE__, os::LOG_FATAL, being, format);
  #define FATAL_(being, format, ...)    os::log_(__FILE__, __LINE__, os::LOG_FATAL, being, format, __VA_ARGS__);

  #define SILENT(being, format)         os::log(__FILE__, __LINE__, os::LOG_SILENT, being, format);
  #define SILENT_(being, format, ...)   os::log_(__FILE__, __LINE__, os::LOG_SILENT, being, format, __VA_ARGS__);

  /////////////////////////////////////////////////////////////////////////////
  // FILESYSTEM PATHS
  /////////////////////////////////////////////////////////////////////////////
  std::string   avatar_glob_pattern(void);
  std::string   zone_glob_pattern(void);
  std::string   social_glob_pattern(void);
  std::string   disabled_command_glob_pattern(void);

  std::string   filename(Avatar* avatar);
  std::string   filename(Zone* zone);
  std::string   filename(SocialCommand* social);
  std::string   deadpool_directory(void);
  std::string   basename(const std::string path);

  std::vector<std::string> glob(std::string pattern);
  bool                     file_exists(std::string path);

  /////////////////////////////////////////////////////////////////////////////
  // PROGRAM EXIT CODES, etc
  /////////////////////////////////////////////////////////////////////////////
  const unsigned EXIT_NORMAL        = 0;
  const unsigned EXIT_FAILED_BOOT   = 1;
  const unsigned EXIT_FAILED_REBOOT = 2;
  const unsigned EXIT_BAD_OPTION    = 3;
  const unsigned EXIT_ERROR         = 4;

  static const char* REBOOT_FILE = "data/reboot.txt";

};

#endif // #ifndef H_SYMPHONY_OS
