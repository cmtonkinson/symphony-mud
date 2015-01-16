/*
 * $Id: container.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_CONTAINER
#define H_SYMPHONY_CONTAINER

#include <list>
#include <map>
#include <string>
#include <vector>

#define CONTAINER_ALL_W_KEYWORDS  -2
#define CONTAINER_ALL             -1
#define CONTAINER_NONE             0

class Object;

/* abstract base class for object containers */
class Container {
  public:
    Container(void);
    virtual ~Container(void);

    virtual void                remove(Object* object) = 0;
    virtual void                remove(const std::list<Object*>& objects) = 0;
    virtual void                purgeObjects(void) = 0;
    virtual std::list<Object*>  searchObjects(const std::string& q) = 0;

    static std::vector<std::string> parseQuery(const std::string& q, int& multiplier, int& index);
    std::list<Object*>              search(const std::list<Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static std::list<Object*>       search(const std::map<int,Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static bool                     commonSearch(Object* object, const std::vector<std::string>& keywords);
};

#endif // !H_SYMPHONY_CONTAINER
