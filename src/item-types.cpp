
#include "item-types.hpp"
#include "regex.hpp"
#include "being.hpp"

///////////////////////////////////////////////////////////////////////////////
// ARMOR
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CLOTHING
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CONTAINER
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// FOOD
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// FURNITURE
///////////////////////////////////////////////////////////////////////////////
ItemFurniture::ItemFurniture(void):
    _capacity(0),
    _layOn(0),
    _sitAt(0),
    _sitOn(0),
    _standOn(0),
    _current(0) {
  return;
}

ItemFurniture::ItemFurniture(const ItemFurniture& ref):
    _capacity(ref.capacity()),
    _layOn(ref.layOn()),
    _sitAt(ref.sitAt()),
    _sitOn(ref.sitOn()),
    _standOn(ref.standOn()),
    _current(ref.current()) {
  return;
}

ItemFurniture::~ItemFurniture(void) {
  return;
}

bool ItemFurniture::add(Being* being, const EnumInt& position) {
  // check overall capacity...
  if (current() >= capacity()) {
    return false;
  }
  // check specific positional capacities...
  switch (position) {
    case FURN_LAY_ON:
      if (beings()[FURN_LAY_ON].size() >= layOn()) {
        return false;
      }
      break;
    case FURN_SIT_AT:
      if (beings()[FURN_SIT_AT].size() >= sitAt()) {
        return false;
      }
      break;
    case FURN_SIT_ON:
      if (beings()[FURN_SIT_ON].size() >= sitOn()) {
        return false;
      }
      break;
    case FURN_STAND_ON:
      if (beings()[FURN_STAND_ON].size() >= standOn()) {
        return false;
      }
      break;
    default:
      return false;
  }
  // add the being...
  beings()[position].push_back(being);
  current(current()+1);
  return true;
}

bool ItemFurniture::remove(Being* being) {
  std::map<EnumInt,std::list<Being*> >::iterator map_it;
  std::list<Being*>::iterator list_it;
  if (!beings().empty()) {
    for (map_it = beings().begin(); map_it != beings().end(); ++map_it) {
      for (list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it) {
        if (*list_it == being) {
          map_it->second.erase(list_it);
          current(current()-1);
          return true;
        }
      }
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// JEWELRY
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// KEY
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TRASH
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// WEAPON
///////////////////////////////////////////////////////////////////////////////
ItemWeapon::ItemWeapon(void) {
  type().set(0);
  verb().set(0);
  return;
}

ItemWeapon::ItemWeapon(const ItemWeapon& ref):
    _type(ref.type()),
    _verb(ref.verb()),
    _damage(ref.damage()) {
  return;
}

ItemWeapon::~ItemWeapon(void) {
  return;
}
