
#ifndef H_SYMPHONY_MUSCL_AST_BINARY
#define H_SYMPHONY_MUSCL_AST_BINARY

#include "ast.h"

class Datum;

class AstBinaryOperation: public Ast {
  public:
    // constructors...
    AstBinaryOperation(void);
    AstBinaryOperation(const unsigned& type, Ast* left, Ast* right);
    virtual ~AstBinaryOperation(void);

    // public accessors...
    void  left(Ast* left)     { _left = left; }
    Ast*  left(void) const    { return _left; }
    void  right(Ast* right)   { _right = right; }
    Ast*  right(void) const   { return _right; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*  _left;
    Ast*  _right;
};

#endif // H_SYMPHONY_MUSCL_AST_BINARY
