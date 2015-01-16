
#ifndef H_SYMPHONY_MUSCL_AST_OBJ_READ
#define H_SYMPHONY_MUSCL_AST_OBJ_READ

#include "ast.h"

class Datum;
class AstObject;

class AstObjectRead: public Ast {
  public:
    // constructors...
    AstObjectRead(void);
    AstObjectRead(AstObject* object);
    virtual ~AstObjectRead(void);

    // public accessors...
    void                object(AstObject* object)             { _object = object; }
    AstObject*          object(void) const                    { return _object; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstObject*          _object;
};

#endif // H_SYMPHONY_MUSCL_AST_OBJ_READ
