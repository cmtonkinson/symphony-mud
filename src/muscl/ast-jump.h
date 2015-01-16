
#ifndef H_SYMPHONY_MUSCL_AST_JUMP
#define H_SYMPHONY_MUSCL_AST_JUMP

#include "ast.h"

class Datum;

class AstJump: public Ast {
  public:
    // constructors...
    AstJump(void);
    AstJump(const unsigned& type, Ast* expression = NULL);
    virtual ~AstJump(void);

    // public accessors...
    void  expression(Ast* expression)   { _expression = expression; }
    Ast*  expression(void) const        { return _expression; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*  _expression;
};

#endif // H_SYMPHONY_MUSCL_AST_SEND
