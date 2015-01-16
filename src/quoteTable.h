
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
