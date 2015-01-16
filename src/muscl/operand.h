
#ifndef H_SYMPHONY_BONES_OPERAND
#define H_SYMPHONY_BONES_OPERAND

#include <string>
#include "datum.h"

using std::string;

class Symbol;

class Operand {
  public:
    // constructors
    Operand(void);
    ~Operand(void);

    // public accessors
    void          address(const int& address)   { _address = address; }
    int           address(void) const           { return _address; }
    void          value(const Datum& value)     { _value = value; }
    Datum&        value(void)                   { return _value; }
    const Datum&  value(void) const             { return _value; }
    void          symbol(Symbol* symbol)        { _symbol = symbol; }
    Symbol*       symbol(void) const            { return _symbol; }


    // general methods
    bool            empty(void) const;
    void            print(void) const;
    string          serialize(void) const;
    static Operand  unserialize(const char* serialized, int start = 0);

  private:
    int     _address;
    Datum   _value;
    Symbol* _symbol;
};

#endif // End of inclusion guard
