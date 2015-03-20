
#ifndef H_SYMPHONY_MUSCL_AST_ITEM_READ
#define H_SYMPHONY_MUSCL_AST_ITEM_READ

#include "ast.h"

class Datum;
class AstItem;

class AstItemRead: public Ast {
  public:
    // constructors...
    AstItemRead(void);
    AstItemRead(AstItem* item);
    virtual ~AstItemRead(void);

    // public accessors...
    void                item(AstItem* item)             { _item = item; }
    AstItem*          item(void) const                    { return _item; }

    // general methods...
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstItem*          _item;
};

#endif // H_SYMPHONY_MUSCL_AST_ITEM_READ
