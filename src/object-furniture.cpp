
#include "creature.h"
#include "object-furniture.h"

ObjFurniture::ObjFurniture(void):
    _capacity(0),
    _layOn(0),
    _sitAt(0),
    _sitOn(0),
    _standOn(0),
    _current(0) {
  return;
}

ObjFurniture::ObjFurniture(const ObjFurniture& ref):
    _capacity(ref.capacity()),
    _layOn(ref.layOn()),
    _sitAt(ref.sitAt()),
    _sitOn(ref.sitOn()),
    _standOn(ref.standOn()),
    _current(ref.current()) {
  return;
}

ObjFurniture::ObjFurniture(ROW row):
    _capacity(row["furniture_capacity"]),
    _layOn(row["furniture_lay_on"]),
    _sitAt(row["furniture_sit_at"]),
    _sitOn(row["furniture_sit_on"]),
    _standOn(row["furniture_stand_on"]),
    _current(0) {
  return;
}

ObjFurniture::~ObjFurniture(void) {
  return;
}

bool ObjFurniture::add(Creature* creature, const EnumInt& position) {
  // check overall capacity...
  if (current() >= capacity()) {
    return false;
  }
  // check specific positional capacities...
  switch (position) {
    case FURN_LAY_ON:
      if (creatures()[FURN_LAY_ON].size() >= layOn()) {
        return false;
      }
      break;
    case FURN_SIT_AT:
      if (creatures()[FURN_SIT_AT].size() >= sitAt()) {
        return false;
      }
      break;
    case FURN_SIT_ON:
      if (creatures()[FURN_SIT_ON].size() >= sitOn()) {
        return false;
      }
      break;
    case FURN_STAND_ON:
      if (creatures()[FURN_STAND_ON].size() >= standOn()) {
        return false;
      }
      break;
    default:
      return false;
  }
  // add the creature...
  creatures()[position].push_back(creature);
  current(current()+1);
  return true;
}

bool ObjFurniture::remove(Creature* creature) {
  std::map<EnumInt,std::list<Creature*> >::iterator map_it;
  std::list<Creature*>::iterator list_it;
  if (!creatures().empty()) {
    for (map_it = creatures().begin(); map_it != creatures().end(); ++map_it) {
      for (list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it) {
        if (*list_it == creature) {
          map_it->second.erase(list_it);
          current(current()-1);
          return true;
        }
      }
    }
  }
  return false;
}
