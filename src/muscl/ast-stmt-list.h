
#ifndef H_SYMPHONY_MUSCL_AST_STMT_LIST
#define H_SYMPHONY_MUSCL_AST_STMT_LIST

#include "ast.h"

class Datum;

class AstStatementList: public Ast {
  public:
    // constructors...
    AstStatementList(void);
    AstStatementList(Ast* node, AstStatementList* next);
    virtual ~AstStatementList(void);

    // public accessors...
    void                node(Ast* node)               { _node = node; }
    Ast*                node(void) const              { return _node; }
    void                next(AstStatementList* next)  { _next =next; }
    AstStatementList*   next(void) const              { return _next; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*              _node;
    AstStatementList* _next;
};

#endif // H_SYMPHONY_MUSCL_AST_STMT_LIST
