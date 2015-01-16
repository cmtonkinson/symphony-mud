
#ifndef H_SYMPHONY_MUSCL_AST_FUNC_DECL
#define H_SYMPHONY_MUSCL_AST_FUNC_DECL

#include "ast.h"

class AstParameterList;
class AstReference;
class SymbolTable;

class AstFunctionDeclaration: public Ast {
  public:
    // constructors...
    AstFunctionDeclaration(void);
    AstFunctionDeclaration(AstReference* reference, AstParameterList* arguments, Ast* body, SymbolTable* symtab);
    virtual ~AstFunctionDeclaration(void);

    // public accessors...
    void                reference(AstReference* reference)        { _reference = reference; }
    AstReference*       reference(void) const                     { return _reference; }
    void                parameters(AstParameterList* parameters) { _parameters = parameters; }
    AstParameterList*  parameters(void) const                    { return _parameters; }
    void                body(Ast* body)                           { _body = body; }
    Ast*                body(void) const                          { return _body; }
    void                symtab(SymbolTable* symtab)               { _symtab = symtab; }
    SymbolTable*        symtab(void)                              { return _symtab; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference*       _reference;
    AstParameterList*   _parameters;
    Ast*                _body;
    SymbolTable*        _symtab;
};

#endif // H_SYMPHONY_MUSCL_AST_FUNC_DECL
