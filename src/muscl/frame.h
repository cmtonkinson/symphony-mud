/*
 * $Id: frame.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_BONES_FRAME
#define H_SYMPHONY_BONES_FRAME

#include <map>
#include <queue>
#include "datum.h"

typedef std::queue<Datum> ValList;
typedef std::pair<DList*,DListIter> IterPair;
typedef std::map<long,IterPair> IterMap;

class Frame {
  public:
    // constructors
    Frame(void);
    ~Frame(void);

    // public accessors
    void              comparison(const bool& comparison)            { _comparison = comparison; }
    bool              comparison(void) const                        { return _comparison; }
    void              returnAddress(const unsigned& returnAddress)  { _returnAddress = returnAddress; }
    unsigned          returnAddress(void) const                     { return _returnAddress; }
    void              arguments(const ValList& arguments)           { _arguments = arguments; }
    ValList&          arguments(void)                               { return _arguments; }
    const ValList&    arguments(void) const                         { return _arguments; }
    void              params(const ValList& params)                 { _params = params; }
    ValList&          params(void)                                  { return _params; }
    const ValList&    params(void) const                            { return _params; }
    void              elements(const ValList& elements)             { _elements = elements; }
    ValList&          elements(void)                                { return _elements; }
    const ValList&    elements(void) const                          { return _elements; }
    void              iterators(const IterMap& iterators)           { _iterators = iterators; }
    IterMap&          iterators(void)                               { return _iterators; }
    const IterMap&    iterators(void) const                         { return _iterators; }

    // general methods
    void    clear(ValList* list);

    // list iteration methods
    IterPair&     get_pair(const long& address);
    const Datum&  get_iter(const long& address);
    void          iter_new(const long& address, const Datum& thelist);
    void          iter_inc(const long& address);
    bool          iter_end(const long& address);
    void          iter_kill(const long& address);

  private:
    // private members
    bool        _comparison;    // comparison value
    unsigned    _returnAddress; // return address
    ValList     _arguments;     // accumulated arguments (for calling frame)
    ValList     _params;        // value copy of argument list (within called frame)
    ValList     _elements;      // accumulated values to initialize a list
    IterMap     _iterators;     // map of list/iterator pairs (indexed by the address of the iterator symbol)
};

#endif // #ifndef H_SYMPHONY_BONES_FRAME
