
#ifndef H_SYMPHONY_MUSCL_SYMBOL
#define H_SYMPHONY_MUSCL_SYMBOL

#include <list>
#include <string>

#define SYMBOL_ERROR  0
#define SYMBOL_NEW    1 // no semantic meaning yet
#define SYMBOL_CONST  2 // constant
#define SYMBOL_VAR    3 // variable
#define SYMBOL_FUNC   4 // function

class Ast;
class AstIdentifierList;
class Datum;

class Symbol {
  public:
    // Constructors...
    Symbol(void);
    Symbol(const std::string& name, unsigned type = SYMBOL_NEW, int line = -1);
    ~Symbol(void);

    // public accessors...
    void                type(const unsigned& type)          { _type = type; }
    unsigned            type(void) const                    { return _type; }
    void                name(const std::string& name)       { _name = name; }
    std::string         name(void) const                    { return _name; }
    void                synlist(AstIdentifierList* synlist) { _synlist = synlist; }
    AstIdentifierList*  synlist(void) const                 { return _synlist; }
    void                code(Ast* code)                     { _code = code; }
    Ast*                code(void) const                    { return _code; }
    void                offset(const int& offset)           { _offset = offset; }
    int                 offset(void) const                  { return _offset; }
    void                line(const int& line)               { _line = line; }
    int                 line(void) const                    { return _line; }

    // general methods...
    bool                isNew(void) const                   { return type() == SYMBOL_NEW; }
    bool                isConstant(void) const              { return type() == SYMBOL_CONST; }
    bool                isVariable(void) const              { return type() == SYMBOL_VAR; }
    bool                isFunction(void) const              { return type() == SYMBOL_FUNC; }
    const char*         stringType(void) const;
    std::string         dump(void) const;

  private:
    // data...
    unsigned            _type;
    std::string         _name;
    AstIdentifierList*  _synlist; // arg list (for functions)
    Ast*                _code;    // code (for functions)
    int                 _offset;  // address (used during code generation)
    int                 _line;    // line on which the symbol was declared
};

#endif // !H_SYMPHONY_MUSCL_SYMBOL
