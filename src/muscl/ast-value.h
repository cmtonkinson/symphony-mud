
#ifndef H_SYMPHONY_MUSCL_AST_CONSTANT
#define H_SYMPHONY_MUSCL_AST_CONSTANT

#include "ast.h"

class Datum;

class AstValue: public Ast {
  public:
    // constructors...
    AstValue(void);
    AstValue(Datum* value);
    virtual ~AstValue(void);

    // public accessors...
    void    value(Datum* value)     { _value = value; }
    Datum*  value(void) const       { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Datum*  _value;
};

#endif // H_SYMPHONY_MUSCL_AST_CONSTANT
