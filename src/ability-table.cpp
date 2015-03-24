
#include "ability-table.hpp"

void AbilityTable::insert(Ability* ability) {
  abilities().insert(ability);
  abilitiesByName()[ability->name()] = ability;
  return;
}

void AbilityTable::remove(Ability* ability) {
  abilities().erase(ability);
  abilitiesByName().erase(ability->name());
  return;
}

Ability* AbilityTable::find(std::string name) const {
  AbilityMap::const_iterator iter = abilitiesByName().find(name);
  if (iter == abilitiesByName().end()) {
    return NULL;
  } else {
    return iter->second;
  }
}

Ability* AbilityTable::find_skill(std::string name) const {
  Ability* ability = find(name);
  if (ability && ability->is_skill()) return ability;
  return NULL;
}

Ability* AbilityTable::find_spell(std::string name) const {
  Ability* ability = find(name);
  if (ability && ability->is_spell()) return ability;
  return NULL;
}

bool AbilityTable::contains(Ability* ability) const {
  size_t count = abilities().count(ability);
  return count > 0;
}

bool AbilityTable::has(std::string name) const {
  return (find(name) != NULL);
}
