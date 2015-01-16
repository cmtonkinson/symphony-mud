
#ifndef H_SYMPHONY_MUSCL_AST_LOOP
#define H_SYMPHONY_MUSCL_AST_LOOP

#include "ast.h"

class AstReference;
class Datum;

class AstLoop: public Ast {
  public:
    // constructors...
    AstLoop(void);
    AstLoop(const unsigned& type, AstReference* iterator, Ast* expression1, Ast* expression2, Ast* bodycode);
    virtual ~AstLoop(void);

    // public accessors...
    void          iterator(AstReference* iterator)    { _iterator = iterator; }
    AstReference* iterator(void)                      { return _iterator; }
    void          expression1(Ast* expression1)       { _expression1 = expression1; }
    Ast*          expression1(void)                   { return _expression1; }
    void          expression2(Ast* expression2)       { _expression2 = expression2; }
    Ast*          expression2(void)                   { return _expression2; }
    void          bodycode(Ast* bodycode)             { _bodycode = bodycode; }
    Ast*          bodycode(void)                      { return _bodycode; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstReference* _iterator;
    Ast*          _expression1;
    Ast*          _expression2;
    Ast*          _bodycode;
};

#endif // H_SYMPHONY_MUSCL_AST_LOOP
