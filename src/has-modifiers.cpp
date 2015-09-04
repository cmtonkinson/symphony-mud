
#include <vector>
#include "has-modifiers.hpp"
#include "modifier.hpp"
#include "regex.hpp"

HasModifiers::HasModifiers(void) {
  return;
}

HasModifiers::HasModifiers(const HasModifiers& ref) {
  _modifiers = ref.modifiers();
  return;
}

HasModifiers::~HasModifiers(void) {
  return;
}

std::string HasModifiers::serializeModifiers(void) const {
  std::vector<std::string> foo;
  char buf[128];

  for (auto iter : modifiers()) {
    sprintf(buf, "%hu:%d", iter.attribute(), iter.magnitude());
    foo.push_back(buf);
  }

  return Regex::implode("~", foo);
}

void HasModifiers::unserializeModifiers(std::string ser) {
  std::vector<std::string> foo = Regex::explode("~", ser);
  unsigned short attr = 0;
  int mag             = 0;

  modifiers().clear();

  for (auto iter : foo) {
    sscanf(iter.c_str(), "%hu:%d", &attr, &mag);
    add(Modifier(attr, mag));
  }

  return;
}

void HasModifiers::add(Modifier mod) {
  modifiers().push_back(mod);
  modify(mod);
  return;
}

void HasModifiers::remove(Modifier mod) {
  modifiers().remove(mod);
  modify(-mod);
  return;
}

bool HasModifiers::has(Modifier mod) {
  for (auto iter : modifiers()) {
    if (iter == mod) return true;
  }
  return false;
}
