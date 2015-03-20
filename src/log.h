
#ifndef H_SYMPHONY_LOG
#define H_SYMPHONY_LOG

#define SILLY(format, ...)    Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);
#define VERBOSE(format, ...)  Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);
#define INFO(format, ...)     Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);
#define WARN(format, ...)     Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);
#define ERROR(format, ...)    Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);
#define SILENT(format, ...)   Log::log(__FILE__, __LINE__, Log::LEVEL_ERROR, format, __VA_ARGS__);

class Log {
  public:

    static const unsigned LEVEL_SILLY   = 0;
    static const unsigned LEVEL_VERBOSE = 1;
    static const unsigned LEVEL_INFO    = 2;
    static const unsigned LEVEL_WARN    = 3;
    static const unsigned LEVEL_ERROR   = 4;
    static const unsigned LEVEL_SILENT  = 5;

    static const unsigned BUFFER_SIZE = 256;

    static void log(const char* file, unsigned line, unsigned level, const char* format, ...);
    static const char*  stringLevel(unsigned level);

  private:

};

#endif // #ifndef H_SYMPHONY_LOG
