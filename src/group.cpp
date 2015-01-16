/* Symphony MUD copyright 2005-2015 Chris Tonkinson <chris@tonkinson.com> */

#include "creature.h"
#include "display.h"
#include "group.h"
#include "room.h"

Group::Group(void) {
  return;
}

Group::~Group(void) {
  return;
}

void Group::leader(Creature* new_leader) {
  _leader = new_leader;
  return;
}

void Group::add_member(Creature* member) {
  _members.insert(member);
  member->group(this);
  return;
}

void Group::remove_member(Creature* member) {
  _members.erase(member);
  member->group(NULL);
  if (_members.empty()) {
    delete this;
  }
  return;
}

bool Group::is_member(Creature* c) {
  return _members.find(c) != _members.end();
}

void Group::send(std::string format, Creature* creature, void* arg1, void* arg2, const unsigned long& target) {
  std::string message;
  format = Regex::trim(format).append(1, '\n');
  for (std::set<Creature*>::iterator c_it = members().begin(); c_it != members().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == TO_CREATURE  && *c_it != creature)
          || (target == TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == TO_NOTVICT   && (*c_it == creature || *c_it == arg1 || *c_it == arg2))
          || (target == TO_ROOM      && *c_it == creature)
       ) {
      continue;
    }
    (*c_it)->send(Display::formatAction(format.c_str(), creature, arg1, arg2, *c_it));
  }
  return;
}
