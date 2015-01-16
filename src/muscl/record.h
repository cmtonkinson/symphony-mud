
#ifndef H_SYMPHONY_BONES_RECORD
#define H_SYMPHONY_BONES_RECORD

#include "operand.h"

class Datum;
class Symbol;

class Record {
  public:
    // constructors
    Record(void);
    Record(const int& opcode);
    ~Record(void);

    // general methods
    void        print(void) const;
    const char* opcodeToString(void) const;

    // public accessors
    void            number(const int& number)   { _number = number; }
    int             number(void) const          { return _number; }
    void            opcode(const int& opcode)   { _opcode = opcode; }
    int             opcode(void) const          { return _opcode; }
    Operand&        left(void)                  { return _left; }
    const Operand&  left(void) const            { return _left; }
    Operand&        right(void)                 { return _right; }
    const Operand&  right(void) const           { return _right; }

  private:
    int     _number;
    int     _opcode;
    Operand _left;
    Operand _right;
};

#endif // End of inclusion guard
