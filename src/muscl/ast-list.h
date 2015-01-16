
#ifndef H_SYMPHONY_MUSCL_AST_LIST
#define H_SYMPHONY_MUSCL_AST_LIST

#include "ast.h"

class AstExpressionList;
class Datum;

class AstList: public Ast {
  public:
    // constructors...
    AstList(void);
    AstList(AstExpressionList* expressions);
    virtual ~AstList(void);

    // public accessors...
    void                expressions(AstExpressionList* expressions)   { _expressions = expressions; }
    AstExpressionList*  expressions(void) const                       { return _expressions; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstExpressionList*  _expressions;
};

#endif // H_SYMPHONY_MUSCL_AST_LIST
