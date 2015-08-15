
#ifndef SYMPHONY_ENUM_H
#define SYMPHONY_ENUM_H

#include <string>

typedef unsigned int EnumInt;

class EnumTable;

class Enum {
  public:
    // constructors...
    Enum(EnumInt value = 0);
    Enum(const Enum& ref)                 { *this = ref; }
    virtual ~Enum(void);

    // operators...
    void  operator  = (const Enum& ref);
    bool  operator == (const Enum& ref)   { return this->number() == ref.number(); }
    bool  operator  < (const Enum& ref)   { return this->number()  < ref.number(); }
    bool  operator  > (const Enum& ref)   { return this->number()  > ref.number(); }

    // accessors...
    EnumInt     number(void) const;
    std::string string(void) const;
    void        set(const EnumInt& value);
    void        set(const std::string& value);

    // general methods...
    std::string list(void) const;

  protected:
    EnumTable*  _table;

  private:
    EnumInt     _value;
};

#endif // SYMPHONY_ENUM_H
