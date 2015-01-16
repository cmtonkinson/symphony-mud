
#ifndef H_SYMPHONY_MUSCL_SYMTAB
#define H_SYMPHONY_MUSCL_SYMTAB

#include <map>
#include <string>

class Symbol;

class SymbolTable {
  public:
    // Constructors...
    SymbolTable(void);
    SymbolTable(const std::string& name);
    ~SymbolTable(void);

    // Public accessor methods...
    void                                  name(const std::string& name) { _name = name; }
    std::string                           name(void) const              { return _name; }
    std::map<std::string,Symbol*>&        table(void)                   { return _table; }
    const std::map<std::string,Symbol*>&  table(void) const             { return _table; }

    // general methods...
    void        insert(Symbol* symbol);
    void        remove(Symbol* symbol);
    Symbol*     lookup(const std::string& name);
    std::string dump(void) const;

  private:
    std::string                   _name;
    std::map<std::string,Symbol*> _table;
};

#endif // !H_SYMPHONY_MUSCL_SYMTAB
