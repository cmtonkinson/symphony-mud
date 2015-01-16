
#ifndef H_SYMPHONY_MUSCL_AST_DECLARATION
#define H_SYMPHONY_MUSCL_AST_DECLARATION

#include "ast.h"

class AstReference;

class AstDeclaration: public Ast {
  public:
    // constructors...
    AstDeclaration(void);
    AstDeclaration(unsigned type, AstReference* reference, Ast* value);
    virtual ~AstDeclaration(void);

    // public accessors...
    void          reference(AstReference* reference)  { _reference = reference; }
    AstReference* reference(void) const               { return _reference; }
    void          value(Ast* value)                   { _value = value; }
    Ast*          value(void) const                   { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference* _reference;
    Ast*          _value;
};

#endif // H_SYMPHONY_MUSCL_AST_DECLARATION
