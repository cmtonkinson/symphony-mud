
#include "ability.h"
#include "world.h"

Ability::Ability(void) {
  return;
}

Ability::~Ability(void) {
  return;
}

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

bool Ability::is_root(void) const {
  return !has_dependencies();
}

bool Ability::is_leaf(void) const {
  return !has_dependents();
}

void Ability::setup(void) {
  return;
}

bool Ability::accessible(Creature* creature) const {
  return false;
}
