
#include <vector>
#include "being.hpp"
#include "item-set.hpp"
#include "modifier.hpp"
#include "os.hpp"
#include "regex.hpp"
#include "world.hpp"
#include "zone.hpp"

std::string ItemSet::serializeItems(void) const {
  std::vector<std::string> foo;
  for (auto iter : items()) foo.push_back(estring(iter->vnum()));
  return Regex::implode("~", foo);
}

void ItemSet::unserializeItems(std::string ser) {
  std::vector<std::string> vnums = Regex::explode("~", ser);
  std::map<unsigned long,Item*>::iterator found;
  unsigned long vnum             = 0;
  Zone* zone                     = nullptr;

  for (auto iter : vnums) {
    vnum = estring(iter);
    if ((zone = World::Instance().lookup(vnum)) == nullptr) {
      ERROR_(nullptr, "Zone lookup failed for vnum %d while unserializing ItemSet %s.", vnum, ser.c_str());
      return;
    }
    if ((found = zone->items().find(vnum)) == zone->items().end()) {
      ERROR_(nullptr, "Item (vnum %d) lookup failed in Zone %s while unserializing ItemSet %s.", vnum, zone->ident(), ser.c_str());
      return;
    }
    items().insert(found->second);
  }
  return;
}

std::string ItemSet::serializeModifiers(void) const {
  std::vector<std::string> foo;
  char buf[128];
  for (auto iter : modifiers()) {
    sprintf(buf, "%s:%d", Being::attributeToString(iter->attribute()), iter->magnitude());
    foo.push_back(buf);
  }
  return Regex::implode("~", foo);
}

void ItemSet::unserializeModifiers(std::string ser) {
  std::vector<std::string> foo = Regex::explode("~", ser);
  std::vector<std::string> bar;
  unsigned short attr = 0;
  int mag = 0;
  for (auto iter : foo) {
    bar = Regex::explode(":", iter);
    attr = Being::stringToAttribute(bar[0]);
    sscanf(bar[1].c_str(), "%d", &mag);
    modifiers().insert(new Modifier(attr, mag));
  }
  return;
}
