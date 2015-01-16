
#ifndef H_SYMPHONY_MUSCL_AST_OBJECT_WRITE
#define H_SYMPHONY_MUSCL_AST_OBJECT_WRITE

#include "ast.h"

class AstObject;

class AstObjectWrite: public Ast {
  public:
    // constructors...
    AstObjectWrite(void);
    AstObjectWrite(AstObject* object, Ast* value);
    virtual ~AstObjectWrite(void);

    // public accessors...
    void        object(AstObject* object) { _object = object; }
    AstObject*  object(void) const        { return _object; }
    void        value(Ast* value)         { _value = value; }
    Ast*        value(void) const         { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstObject*  _object;
    Ast*        _value;
};

#endif // H_SYMPHONY_MUSCL_AST_OBJECT_WRITE
