
#include "item-types.hpp"
#include "regex.hpp"
#include "being.hpp"
#include "os.hpp"

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
ItemWeapon::ItemWeapon(const Item* base_) {
  type().set(0);
  verb().set(0);
  base(base_);
  keyStat(Being::ATTR_UNDEFINED);
  return;
}

ItemWeapon::ItemWeapon(const Item* base_, const ItemWeapon& ref):
    _type(ref.type()),
    _verb(ref.verb()),
    _damage(ref.damage()),
    _base(base_),
    _key_stat(ref.keyStat()) {
  return;
}

ItemWeapon::~ItemWeapon(void) {
  return;
}

double ItemWeapon::weightFactor(void) const {
  double density = 1.0;
  double size    = 3.0;

  if (base()->compound() != nullptr) density = base()->compound()->density();
  size = relativeSize(this);

  return density * size;
}

double ItemWeapon::relativeSize(const ItemWeapon* weapon) {
  switch (weapon->type().number()) {
    case WEAP_AXE:          return 3.0;
    case WEAP_CLUB:         return 3.0;
    case WEAP_DAGGER:       return 1.0;
    case WEAP_FLAIL:        return 3.0;
    case WEAP_GAUNTLET:     return 1.0;
    case WEAP_LONGSWORD:    return 5.0;
    case WEAP_MACE:         return 4.0;
    case WEAP_POLEARM:      return 5.0;
    case WEAP_SHORTSWORD:   return 3.0;
    case WEAP_STAFF:        return 4.0;
    case WEAP_WHIP:         return 2.0;
    default:
      ERROR_(nullptr, "ItemWeapon::relativeSize() - invalid weapon type (%d) for vnum %lu '%s'", weapon->type().number(), weapon->base()->vnum(), weapon->base()->name());
      return 3.0;
  }
}

double ItemWeapon::relativeRange(const ItemWeapon* weapon) {
  switch (weapon->type().number()) {
    case WEAP_AXE:          return 2.0;
    case WEAP_CLUB:         return 2.0;
    case WEAP_DAGGER:       return 1.0;
    case WEAP_FLAIL:        return 3.0;
    case WEAP_GAUNTLET:     return 1.0;
    case WEAP_LONGSWORD:    return 4.0;
    case WEAP_MACE:         return 2.0;
    case WEAP_POLEARM:      return 5.0;
    case WEAP_SHORTSWORD:   return 2.0;
    case WEAP_STAFF:        return 4.0;
    case WEAP_WHIP:         return 4.0;
    default:
      ERROR_(nullptr, "ItemWeapon::relativeRange() - invalid weapon type (%d) for vnum %lu '%s'", weapon->type().number(), weapon->base()->vnum(), weapon->base()->name());
      return 3.0;
  }
}
