
#ifndef H_SYMPHONY_MUSCL_AST_FUNC_CALL
#define H_SYMPHONY_MUSCL_AST_FUNC_CALL

#include "ast.h"

class AstExpressionList;
class AstReference;

class AstFunctionCall: public Ast {
  public:
    // constructors...
    AstFunctionCall(void);
    AstFunctionCall(AstReference* reference, AstExpressionList* arguments);
    virtual ~AstFunctionCall(void);

    // public accessors...
    void                reference(AstReference* reference)      { _reference = reference; }
    AstReference*       reference(void) const                   { return _reference; }
    void                arguments(AstExpressionList* arguments) { _arguments = arguments; }
    AstExpressionList*  arguments(void) const                   { return _arguments; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference*       _reference;
    AstExpressionList*  _arguments;
};

#endif // H_SYMPHONY_MUSCL_AST_FUNC_DECL
