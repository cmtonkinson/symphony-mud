
#ifndef H_SYMPHONY_OS
#define H_SYMPHONY_OS

#include <string>

class Avatar;
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
  const unsigned LOG_SILLY   = 0;
  const unsigned LOG_VERBOSE = 1;
  const unsigned LOG_INFO    = 2;
  const unsigned LOG_WARN    = 3;
  const unsigned LOG_ERROR   = 4;
  const unsigned LOG_SILENT  = 5;

  void          log(const char* file, unsigned line, unsigned level, const char* format, ...);
  const char*   stringLevel(unsigned level);

  // Remember, #defines within a namespace will still bind to global scope.
  #define SILLY(format, ...)    os::log(__FILE__, __LINE__, os::LOG_SILLY, format, __VA_ARGS__);
  #define VERBOSE(format, ...)  os::log(__FILE__, __LINE__, os::LOG_VERBOSE, format, __VA_ARGS__);
  #define INFO(format, ...)     os::log(__FILE__, __LINE__, os::LOG_INFO, format, __VA_ARGS__);
  #define WARN(format, ...)     os::log(__FILE__, __LINE__, os::LOG_WARN, format, __VA_ARGS__);
  #define ERROR(format, ...)    os::log(__FILE__, __LINE__, os::LOG_ERROR, format, __VA_ARGS__);
  #define SILENT(format, ...)   os::log(__FILE__, __LINE__, os::LOG_SILENT, format, __VA_ARGS__);

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

  std::vector<std::string> glob(std::string pattern);
  bool                     file_exists(std::string path);

};

#endif // #ifndef H_SYMPHONY_OS
