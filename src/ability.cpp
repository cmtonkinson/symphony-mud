
#include "ability.h"
#include "world.h"

void Ability::add_dependency(Ability* ability) {
  _dependencies.insert(ability);
  ability->dependents().insert(this);
  return;
}

void Ability::add_dependent(Ability* ability) {
  _dependents.insert(ability);
  ability->dependencies().insert(this);
  return;
}

bool Ability::has_dependencies(void) const {
  return !_dependencies.empty();
}

bool Ability::has_dependents(void) const {
  return !_dependents.empty();
}

std::set<std::string> Ability::dependency_names(void) const {
  std::set<Ability*>::const_iterator iter;
  std::set<std::string> prereqs;
  for (iter = dependencies().begin(); iter != dependencies().end(); ++iter) {
    prereqs.insert((*iter)->name());
  }
  return prereqs;
}

bool Ability::is_root(void) const {
  return !has_dependencies();
}

bool Ability::is_leaf(void) const {
  return !has_dependents();
}
