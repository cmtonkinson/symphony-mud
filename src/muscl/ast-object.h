
#ifndef H_SYMPHONY_MUSCL_AST_ITEM
#define H_SYMPHONY_MUSCL_AST_ITEM

#include "ast.h"
#include <string>
#include <queue>

using std::queue;

class Datum;

class AstItem: public Ast {
  public:
    // constructors...
    AstItem(void);
    AstItem(const std::string& name);
    virtual ~AstItem(void);

    // public accessors...
    void                parent(AstItem* const parent)         { _parent = parent; }
    AstItem*          parent(void) const                      { return _parent; }
    void                child(AstItem* const child)           { _child = child; }
    AstItem*          child(void) const                       { return _child; }
    Datum*              name(void) const                        { return _name; }
    std::string         fullName(void) const;

    // general methods...
    void                addChild(AstItem* child);
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstItem*  _parent;
    AstItem*  _child;
    Datum*      _name;
};

#endif // H_SYMPHONY_MUSCL_AST_ITEM
