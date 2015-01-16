/*
 * $Id: quoteTable.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_QUOTE_TABLE
#define H_SYMPHONY_QUOTE_TABLE

#include <stdlib.h>
#include <string>
#include <vector>

typedef std::pair<std::string,std::string>  Quote;
typedef std::vector<Quote>                  QuoteList;



class QuoteTable {

  public:
    QuoteTable(void);
    ~QuoteTable(void);

    // Public accessor methods...
    QuoteList&        quotes(void)               { return _quotes; }
    const QuoteList&  quotes(void) const         { return _quotes; }

    // General methods...
    void              add(const std::string& author, const std::string& quote);
    Quote             getRandomQuote(void) const;

    // Static methods...
    static QuoteTable&  Instance(void);

  private:
    QuoteList _quotes;

};

inline QuoteTable& QuoteTable::Instance(void) { // public static
  static QuoteTable instance;
  return instance;
}



#endif // #ifndef H_SYMPHONY_QUOTE_TABLE
