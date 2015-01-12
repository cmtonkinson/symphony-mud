
#include "ability.h"
#include "world.h"

Ability::Ability(void) {
  return;
}

Ability::~Ability(void) {
  return;
}

void Ability::add_dependency(std::string dep_name) {
  std::string message;
  Ability* a = World::Instance().abilities().find(dep_name);
  if (a == NULL) {
    fprintf(stderr, "Bad dependency of %s (%s could not be found).\n", name().c_str(), dep_name.c_str());
  } else {
    add_dependency(a);
  }
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
