
#ifndef H_SYMPHONY_MUSCL_AST_MAP_PAIR
#define H_SYMPHONY_MUSCL_AST_MAP_PAIR

#include "ast.h"

class Datum;

class AstMapPair: public Ast {
  public:
    // constructors...
    AstMapPair(void);
    AstMapPair(Ast* key, Ast* value);
    virtual ~AstMapPair(void);

    // public accessors...
    void  key(Ast* key)     { _key = key; }
    Ast*  key(void) const    { return _key; }
    void  value(Ast* value)   { _value = value; }
    Ast*  value(void) const   { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    Ast*  _key;
    Ast*  _value;
};

#endif // H_SYMPHONY_MUSCL_AST_MAP_PAIR
