
#ifndef H_SYMPHONY_MUSCL_AST_ITEM_WRITE
#define H_SYMPHONY_MUSCL_AST_ITEM_WRITE

#include "ast.h"

class AstItem;

class AstItemWrite: public Ast {
  public:
    // constructors...
    AstItemWrite(void);
    AstItemWrite(AstItem* item, Ast* value);
    virtual ~AstItemWrite(void);

    // public accessors...
    void        item(AstItem* item) { _item = item; }
    AstItem*  item(void) const        { return _item; }
    void        value(Ast* value)         { _value = value; }
    Ast*        value(void) const         { return _value; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstItem*  _item;
    Ast*        _value;
};

#endif // H_SYMPHONY_MUSCL_AST_ITEM_WRITE
