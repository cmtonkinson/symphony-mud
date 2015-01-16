
#ifndef H_REGEX
#define H_REGEX

#include <cstring>
#include <list>
#include <map>
#include <pcre.h>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

#define REGEX_DEFAULT_OPTIONS     (PCRE_CASELESS|PCRE_DOTALL|PCRE_MULTILINE)
#define REGEX_MAX_SUBPATTERNS     20 // Maximum number of supatterns to capture
#define REGEX_OVECTOR_SIZE        REGEX_MAX_SUBPATTERNS*3
#define REGEX_CACHE_SIZE          4 // Maximum number of compiled regular expressions to store
//#define REGEX_THREAD_SAFE         1
#ifdef REGEX_THREAD_SAFE

#include <pthread.h>
#endif

typedef std::vector<std::pair<std::string,std::vector<std::string> > > match_list;



//////////////////////////////////////////////////////////////////////////  REGEX EXCEPTION  //////////////////////////////////////////////////////////////////////////
class RegexException {
  private:
    std::string   _message;

  public:
    RegexException(void);
    RegexException(const std::string& file, const unsigned long& line, const std::string& extra = "");
    RegexException(const RegexException& ref);
    ~RegexException(void);

    // Public accessor methods...
    void          setMessage(const std::string& message)      { _message = message; }
    std::string   getMessage(void) const                      { return _message; }
};

//////////////////////////////////////////////////////////////////////////  REGEX INFO  //////////////////////////////////////////////////////////////////////////
class RegexInfo {
  private:
    // Private methods...
    std::string       _pattern;
    pcre*             _code;
    int               _options;
    pcre_extra*       _extra;

    // Private members...
    void              pattern(const std::string& pattern)     { _pattern = pattern; }
    void              code(pcre* code)                        { _code = code; }
    void              options(const int& options)             { _options = options; }
    void              extra(pcre_extra* extra)                { _extra = extra; }

  public:
    // Constructors...
    RegexInfo(std::pair<std::string,int>* pair, const bool& study);
    ~RegexInfo(void);

    // Public accessor methods...
    std::string       pattern(void) const                     { return _pattern; }
    pcre*             code(void) const                        { return _code; }
    int               options(void) const                     { return _options; }
    pcre_extra*       extra(void) const                       { return _extra; }

};

//////////////////////////////////////////////////////////////////////////  REGEX  //////////////////////////////////////////////////////////////////////////
class Regex {
  private:
    static std::map<std::pair<std::string,int>,RegexInfo*>                        _cache;
    static std::list<std::map<std::pair<std::string,int>,RegexInfo*>::iterator>   _cache_meta;
    #ifdef REGEX_THREAD_SAFE
    static pthread_mutex_t              _mutex;
    static pthread_mutexattr_t          _attributes;
    static void                         cleanup_thread_stuff(void);
    #endif

    static bool                         execute(pcre* code, pcre_extra* extra, const std::string& subject, int& offset, int* ovector);
    static unsigned int                 min(const unsigned int& a, const unsigned int& b);

  public:
    Regex(void);
    ~Regex(void);

    #ifdef REGEX_THREAD_SAFE
    static void                         init(void);
    #endif
    static RegexInfo*                   get(const std::string& pattern, const int& options, const bool& study);

    /* True regular expression methods (which actually use PCRE) */
    static bool                         match(const std::string& pattern, const std::string& subject,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);
    static bool                         match(const std::string& pattern, const std::string& subject, std::string& match,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);
    static bool                         match(const std::string& pattern, const std::string& subject, std::vector<std::string>& matches,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);
    static bool                         match(const std::string& pattern, const std::string& subject, match_list& matches,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);
    static bool                         replace(const std::string& pattern, const std::string& replacement, std::string& subject,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);
    static bool                         replace(const std::string& pattern, const std::string& replacement, const std::string& subject, std::string& dest,
                                            int options = REGEX_DEFAULT_OPTIONS, bool study = true);

    /* Other string manipulation methods for convenience */
    static unsigned int                 count(const std::string& needle, const std::string& haystack);
    static std::string                  ltrim(const std::string& target);
    static std::string                  rtrim(const std::string& target);
    static std::string                  trim(const std::string& target);
    static std::string                  strip(const std::string& target, const char& kill);
    static std::string                  lower(const std::string& target);
    static std::string                  upper(const std::string& target);
    static std::vector<std::string>     explode(const std::string& delimiter, const std::string& target, bool trim_substrings = false);
    static std::vector<std::string>     explode_multi(const std::string& delimiters, const std::string& target, bool trim_substrings = false);
    static std::string                  implode(const std::string& glue, const std::vector<std::string>& pieces);
    static std::string                  implode(const std::string& glue, const std::set<std::string>& pieces);
    static std::vector<std::string>     split(const std::string& src, unsigned short n = 1);
    static bool                         strPrefix(const std::string& prefix, const std::string& str);
    static bool                         isMisspelling(const std::string& attempt, const std::string& correct);
    static unsigned int                 levenshtein(const std::string& string1, const std::string& string2);
    static std::string                  encodeEmail(const std::string& target);
    static std::string                  decodeEmail(const std::string& target);
};



#endif // #ifndef H_REGEX
