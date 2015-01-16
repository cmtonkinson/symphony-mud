
#ifndef H_SYMPHONY_MUSCL_AST_UNARY
#define H_SYMPHONY_MUSCL_AST_UNARY

#include "ast.h"

class Datum;

class AstUnaryOperation: public Ast {
  public:
    // constructors...
    AstUnaryOperation(void);
    AstUnaryOperation(const unsigned& type, Ast* node);
    virtual ~AstUnaryOperation(void);

    // public accessors...
    void  node(Ast* node)   { _node = node; }
    Ast*  node(void) const  { return _node; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*    _node;
};

#endif // H_SYMPHONY_MUSCL_AST_UNARY
