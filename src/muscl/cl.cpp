
#include "cl.h"

ContextLayer::ContextLayer(void) {
  return;
}

ContextLayer::~ContextLayer(void) {
  return;
}

bool ContextLayer::isValidReference(const string& name) {
  return true;
}

bool ContextLayer::readable(const string& name) const {
  ContextNode* cn = resolve(name);
  if (cn && cn->readable()) return true;
  return false;
}

bool ContextLayer::writable(const string& name) const {
  ContextNode* cn = resolve(name);
  if (cn && cn->writable()) return true;
  return false;
}

bool ContextLayer::read(const string& name, Datum& value) {
  ContextNode* cn = resolve(name);
  if (cn) return cn->read(value);
  return false;
}

bool ContextLayer::write(const string& name, const Datum& value) {
  ContextNode* cn = resolve(name);
  if (cn) return cn->write(value);
  return false;
}

bool ContextLayer::add(const string& name, ContextNode* node) {
  ContextMap::const_iterator it;
  bool overwriting = false;
  if ((it = _items.find(name)) != _items.end()) {
    overwriting = true;
    remove(name);
  }
  _items[name] = node;
  return overwriting;
}

bool ContextLayer::remove(const string& name) {
  ContextMap::const_iterator it;
  if ((it = _items.find(name)) != _items.end()) {
    delete it->second;
    _items.erase(name);
    return true;
  }
  return false;
}

ContextNode* ContextLayer::resolve(const string& name) const {
  ContextMap::const_iterator it;
  if ((it = _items.find(name)) != _items.end()) return it->second;
  return NULL;
}
