
#ifndef H_SYMPHONY_MUSCL_AST_EXPR_LIST
#define H_SYMPHONY_MUSCL_AST_EXPR_LIST

#include "ast.h"

class Datum;

class AstExpressionList: public Ast {
  public:
    // constructors...
    AstExpressionList(void);
    AstExpressionList(Ast* node, AstExpressionList* next);
    virtual ~AstExpressionList(void);

    // public accessors...
    void                node(Ast* node)               { _node = node; }
    Ast*                node(void) const              { return _node; }
    void                next(AstExpressionList* next) { _next = next; }
    AstExpressionList*  next(void) const              { return _next; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*                _node;
    AstExpressionList*  _next;
};

#endif // H_SYMPHONY_MUSCL_AST_EXPR_LIST
