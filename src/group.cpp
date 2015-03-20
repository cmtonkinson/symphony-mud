
#include "being.h"
#include "display.h"
#include "group.h"
#include "room.h"

Group::Group(void) {
  return;
}

Group::~Group(void) {
  return;
}

void Group::leader(Being* new_leader) {
  _leader = new_leader;
  return;
}

void Group::add_member(Being* member) {
  _members.insert(member);
  member->group(this);
  return;
}

void Group::remove_member(Being* member) {
  _members.erase(member);
  member->group(NULL);
  if (_members.empty()) {
    delete this;
  }
  return;
}

bool Group::is_member(Being* b) {
  return _members.find(b) != _members.end();
}

void Group::send(std::string format, Being* being, void* arg1, void* arg2, const unsigned long& target) {
  std::string message;
  format = Regex::trim(format).append(1, '\n');
  for (std::set<Being*>::iterator c_it = members().begin(); c_it != members().end(); ++c_it) {
    // Skip if the target is wrong...
    if  (   (target == Room::TO_BEING  && *c_it != being)
          || (target == Room::TO_VICT      && (*c_it != arg1 || *c_it != arg2))
          || (target == Room::TO_NOTVICT   && (*c_it == being || *c_it == arg1 || *c_it == arg2))
          || (target == Room::TO_ROOM      && *c_it == being)
       ) {
      continue;
    }
    (*c_it)->send(Display::formatAction(format.c_str(), being, arg1, arg2, *c_it));
  }
  return;
}
