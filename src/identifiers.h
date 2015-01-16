/*
 * $Id: identifiers.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_SYMPHONY_IDENTIFIERS
#define H_SYMPHONY_IDENTIFIERS

#include <set>
#include <string>
#include "color-string.h"
#include "mysql.h"
#include "regex.h"

/*
 * This class is designed to "kind of" emulate an interface to
 * be implemented by classes which will have short and long names,
 * be accessible via keywords, and contain long descriptionns.
 */
class Identifiers {
  public:
    // Constructors...
    Identifiers(void);
    Identifiers(ROW row);
    Identifiers(const Identifiers& ref);
    ~Identifiers(void);

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

  private:
    std::string                   _shortname;
    std::string                   _longname;
    std::string                   _description;
    std::set<std::string>         _keywords;
};

#endif // #ifndef H_SYMPHONY_IDENTIFIERS
