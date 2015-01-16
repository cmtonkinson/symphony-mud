
#ifndef H_SYMPHONY_MUSCL_AST_OBJECT
#define H_SYMPHONY_MUSCL_AST_OBJECT

#include "ast.h"
#include <string>
#include <queue>

using std::queue;

class Datum;

class AstObject: public Ast {
  public:
    // constructors...
    AstObject(void);
    AstObject(const std::string& name);
    virtual ~AstObject(void);

    // public accessors...
    void                parent(AstObject* const parent)         { _parent = parent; }
    AstObject*          parent(void) const                      { return _parent; }
    void                child(AstObject* const child)           { _child = child; }
    AstObject*          child(void) const                       { return _child; }
    Datum*              name(void) const                        { return _name; }
    std::string         fullName(void) const;

    // general methods...
    void                addChild(AstObject* child);
    virtual Datum*      eval(void);
    virtual void        print(std::string indent, bool last);
    virtual InstrList&  codegen(void);

  private:
    // data...
    AstObject*  _parent;
    AstObject*  _child;
    Datum*      _name;
};

#endif // H_SYMPHONY_MUSCL_AST_OBJECT
