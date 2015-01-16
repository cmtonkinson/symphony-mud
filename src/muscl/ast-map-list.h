
#ifndef H_SYMPHONY_MUSCL_AST_MAP_LIST
#define H_SYMPHONY_MUSCL_AST_MAP_LIST

#include "ast.h"

class Datum;

class AstMapList: public Ast {
  public:
    // constructors...
    AstMapList(void);
    AstMapList(Ast* node, AstMapList* next);
    virtual ~AstMapList(void);

    // public accessors...
    void          node(Ast* node)               { _node = node; }
    Ast*          node(void) const              { return _node; }
    void          next(AstMapList* next) { _next = next; }
    AstMapList*   next(void) const              { return _next; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*          _node;
    AstMapList*   _next;
};

#endif // H_SYMPHONY_MUSCL_AST_MAP_LIST
