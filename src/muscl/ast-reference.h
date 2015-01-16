
#ifndef H_SYMPHONY_MUSCL_AST_REFERENCE
#define H_SYMPHONY_MUSCL_AST_REFERENCE

#include "ast.h"

class Datum;
class Symbol;

class AstReference: public Ast {
  public:
    // constructors...
    AstReference(void);
    AstReference(Symbol* symbol);
    virtual ~AstReference(void);

    // public accessors...
    void                symbol(Symbol* symbol)                    { _symbol = symbol; }
    Symbol*             symbol(void) const                        { return _symbol; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Symbol*           _symbol;
};

#endif // H_SYMPHONY_MUSCL_AST_REFERENCE
