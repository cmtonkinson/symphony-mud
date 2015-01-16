
#ifndef H_SYMPHONY_MUSCL_AST_SEND
#define H_SYMPHONY_MUSCL_AST_SEND

#include "ast.h"

class Datum;

class AstSend: public Ast {
  public:
    // constructors...
    AstSend(void);
    AstSend(Ast* node);
    virtual ~AstSend(void);

    // public accessors...
    void  node(Ast* node)     { _node = node; }
    Ast*  node(void) const    { return _node; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*  _node;
};

#endif // H_SYMPHONY_MUSCL_AST_SEND
