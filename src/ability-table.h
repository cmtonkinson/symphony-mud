#ifndef H_SYMPHONY_ABILITY_TABLE
#define H_SYMPHONY_ABILITY_TABLE

#include <map>
#include <set>
#include <string>
#include "ability.h"

typedef std::map<std::string,Ability*> AbilityMap;

class AbilityTable {
  public:
    AbilityTable(void);
    ~AbilityTable(void);

    std::set<Ability*>&             abilities()               { return _abilities; }
    const std::set<Ability*>&       abilities() const         { return _abilities; }
    AbilityMap&                     abilitiesByName()         { return _abilities_by_name; }
    const AbilityMap&               abilitiesByName() const   { return _abilities_by_name; }

    void      insert(Ability* ability);
    void      remove(Ability* ability);
    Ability*  find(std::string name) const;
    bool      contains(Ability* ability) const;
    bool      has(std::string name) const;

  private:
    std::set<Ability*>  _abilities;
    AbilityMap          _abilities_by_name;

};

#endif // #ifndef H_SYMPHONY_ABILITY_TABLE
