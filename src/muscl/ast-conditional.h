
#ifndef H_SYMPHONY_MUSCL_AST_CONDITIONAL
#define H_SYMPHONY_MUSCL_AST_CONDITIONAL

#include "ast.h"

class Datum;

class AstConditional: public Ast {
  public:
    // constructors...
    AstConditional(void);
    AstConditional(const unsigned& type, Ast* condition, Ast* bodycode, Ast* elsecode);
    virtual ~AstConditional(void);

    // public accessors...
    void      condition(Ast* condition)           { _condition = condition; }
    Ast*      condition(void) const               { return _condition; }
    void      bodycode(Ast* bodycode)             { _bodycode = bodycode; }
    Ast*      bodycode(void)                      { return _bodycode; }
    void      elsecode(Ast* elsecode)             { _elsecode = elsecode; }
    Ast*      elsecode(void)                      { return _elsecode; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*      _condition; // test condition
    Ast*      _bodycode;  // main code
    Ast*      _elsecode;  // optional "else" code (for if/else)
};

#endif // H_SYMPHONY_MUSCL_AST_CONDITIONAL
