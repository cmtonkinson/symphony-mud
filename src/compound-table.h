
#ifndef H_SYMPHONY_COMPOUND_TABLE
#define H_SYMPHONY_COMPOUND_TABLE

#include <map>
#include <string>
#include "compound.h"

class CompoundTable {
  private:
    std::map<std::string,Compound*>    _compounds;

  public:
    CompoundTable(void);
    ~CompoundTable(void);

    // Public accessor methods...
    std::map<std::string,Compound*>&         compounds(void)               { return _compounds; }
    const std::map<std::string,Compound*>&   compounds(void) const         { return _compounds; }

    // General methods...
    void        add(Compound*);
    Compound*   find(const std::string& name) const;
    std::string list(std::string glue = " ") const;

    static CompoundTable&  Instance(void);

};

inline CompoundTable& CompoundTable::Instance(void) { // public static
  static CompoundTable instance;
  return instance;
}

#endif // #ifndef H_SYMPHONY_COMPOUND_TABLE
