
#ifndef H_SYMPHONY_CURRENCY
#define H_SYMPHONY_CURRENCY

#include <string>

class Currency {
  public:
    static const unsigned SILVER_PER_GOLD = 1000;

    Currency(void);
    Currency(unsigned value);
    Currency(const Currency& other);
    ~Currency(void);

    // Comparison operators
    bool operator == (const Currency& other) const  { return value() == other.value(); }
    bool operator != (const Currency& other) const  { return value() != other.value(); }
    bool operator >  (const Currency& other) const  { return value() >  other.value(); }
    bool operator >= (const Currency& other) const  { return value() >= other.value(); }
    bool operator <  (const Currency& other) const  { return value() <  other.value(); }
    bool operator <= (const Currency& other) const  { return value() <= other.value(); }

    // Manipulation operators
    operator unsigned (void) const                  { return value(); }
    Currency& operator = (const Currency& other)    { value(other.value()); return *this; }
    Currency  operator + (const Currency& other)    { return Currency(value() +  other.value()); }
    Currency  operator - (const Currency& other)    { return Currency(value() -  other.value()); }
    Currency  operator / (const Currency& other)    { return Currency(value() /  other.value()); }
    Currency  operator * (const Currency& other)    { return Currency(value() *  other.value()); }

    // Accessors
    unsigned  value(void) const     { return _value; }
    void      value(unsigned value) { _value = value; }

    // General methods
    unsigned    silver(void) const;
    unsigned    gold(void) const;
    std::string shortString(void) const;
    std::string longString(void) const;

  private:
    unsigned _value;
};

#endif // #ifndef H_SYMPHONY_CURRENCY
