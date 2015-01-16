
#ifndef H_SYMPHONY_MUSCL_AST_PARAM_LIST
#define H_SYMPHONY_MUSCL_AST_PARAM_LIST

#include "ast.h"

class AstReference;
class Datum;

class AstParameterList: public Ast {
  public:
    // constructors...
    AstParameterList(void);
    AstParameterList(AstReference* node, AstParameterList* next);
    virtual ~AstParameterList(void);

    // public accessors...
    void                ref(AstReference* ref)        { _ref = ref; }
    AstReference*       ref(void) const               { return _ref; }
    void                next(AstParameterList* next)  { _next = next; }
    AstParameterList*   next(void) const              { return _next; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference*       _ref;
    AstParameterList*   _next;
};

#endif // H_SYMPHONY_MUSCL_AST_PARAM_LIST
