
#ifndef H_SYMPHONY_MUSCL_SCOPE
#define H_SYMPHONY_MUSCL_SCOPE

#include <list>
#include <string>

class Symbol;
class SymbolTable;

/* Scope::_frames is a list that gets treated like a stack at times.
 * This list implements basic scoping. The front of the list is equivalent
 * to the "top of stack" for scopes. When inserting or removing a symbol,
 * only the "top of stack" is ever affected. When searching for a symbol,
 * each SymbolTable in the list is visited (front to back) until a match
 * is found.
 *
 * This acts like a stack when mutating symbols and adding/removing scopes,
 * with the exception that when searching for a symbol, if the name isn't
 * found in the inner-most (top of stack/front of list) SymbolTable, the
 * search moves on to the next inner-most SymbolTable.
 *
 * Ideally, muscl will implement a default scope that's loaded with
 * constants (e.g. 'pi') and built-in functions (e.g. 'rand()'). A fresh
 * [empty by default] SymbolTable is then pushed for the script at hand.
 * Every function call from there pushes yet a new SymbolTable into scope.
 * The system is designed to abort if there are no SymbolTables in scope
 * (at any given time during script execution, there should be at least 2:
 * the default scope and the script's global scope).
 */
class Scope {
  public:
    // Constructors...
    Scope(void);
    ~Scope(void);

    // public accessor methods
    std::list<SymbolTable*>&        frames(void)        { return _frames; }
    const std::list<SymbolTable*>&  frames(void) const  { return _frames; }

    // general methods
    void          check(void);                        // ensures that there is at least one SymbolTable in scope
    void          pushFrame(const std::string& name); // add a new scope frame
    SymbolTable*  popFrame(void);                     // remove the top scope frame
    void          insert(Symbol* symbol);             // create a symbol in the current scope
    void          remove(Symbol* symbol);             // remove a symbol from the current scope
    Symbol*       lookup(const std::string& name);    // return the top-most symbol matching name
    std::string   dump(void) const;

  private:
    std::list<SymbolTable*>   _frames;
};

#endif // !H_SYMPHONY_MUSCL_SCOPE
