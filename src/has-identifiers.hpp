
#ifndef H_SYMPHONY_HAS_IDENTIFIERS
#define H_SYMPHONY_HAS_IDENTIFIERS

#include <set>
#include <string>
#include "color-string.hpp"

class HasIdentifiers {
  public:
    // Constructors...
    HasIdentifiers(void);
    HasIdentifiers(const HasIdentifiers& ref);
    virtual ~HasIdentifiers(void);

    // Public accessor methods...
    void                          shortname(const std::string& shortname)             { _shortname.assign(shortname); }
    std::string                   shortname(void) const                               { return _shortname; }
    void                          longname(const std::string& longname)               { _longname.assign(ColorString(longname).stripColor()); }
    std::string                   longname(void) const                                { return _longname; }
    void                          description(const std::string& description)         { _description.assign(description); }
    std::string                   description(void) const                             { return _description; }
    std::string*                  descriptionp(void)                                  { return &_description; } // this needs to be exposed for the text editor
    std::set<std::string>&        getKeywords(void)                                   { return _keywords; }
    const std::set<std::string>&  getKeywords(void) const                             { return _keywords; }
    void                          setKeywords(const std::set<std::string>& keywords)  { _keywords = keywords; }

    // General methods...
    std::string                   getKeywordList(std::string glue = " ") const;
    void                          addKeyword(const std::string& keyword);
    void                          removeKeyword(const std::string& keyword);
    bool                          matchesKeyword(const std::string& keyword) const;
    std::string                   serializeKeywords(void) const;
    void                          unserializeKeywords(const std::string& serialization);
    std::string                   longestKeyword(void) const;

    // Shortcuts...
    const char*   name(void) const  { return shortname().c_str(); }

  private:
    std::string                   _shortname;
    std::string                   _longname;
    std::string                   _description;
    std::set<std::string>         _keywords;
};

#endif // #ifndef H_SYMPHONY_HAS_IDENTIFIERS
