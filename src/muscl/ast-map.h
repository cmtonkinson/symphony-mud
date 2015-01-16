
#ifndef H_SYMPHONY_MUSCL_AST_MAP
#define H_SYMPHONY_MUSCL_AST_MAP

#include "ast.h"

class AstMapList;
class Datum;

class AstMap: public Ast {
  public:
    // constructors...
    AstMap(void);
    AstMap(AstMapList* pairs);
    virtual ~AstMap(void);

    // public accessors...
    void          pairs(AstMapList* pairs)    { _pairs = pairs; }
    AstMapList*   pairs(void) const           { return _pairs; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstMapList*  _pairs;
};

#endif // H_SYMPHONY_MUSCL_AST_MAP
