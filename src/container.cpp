
#include "container.h"
#include "estring.h"
#include "item.h"

Container::Container(void) {
  return;
}

Container::~Container(void) {
  return;
}

std::vector<std::string> Container::parseQuery(const std::string& q, int& multiplier, int& index) {
  std::string query = Regex::trim(Regex::lower(Regex::strip(q, '\'')));
  std::vector<std::string> keywords;
  size_t pos = 0;

  if (query == "all") {
    // return all items
    multiplier = ALL;
    index = NONE;
  } else if (Regex::strPrefix("all*", query)) {
    // return all of the matched items
    multiplier = ALL_W_KEYWORDS;
    index = NONE;
    keywords = Regex::explode(" ", query.substr(4), true);
  } else if ((pos = query.find('*')) != std::string::npos) {
    // return a certain number of the matched items
    multiplier = estring(query.substr(0, pos));
    index = NONE;
    keywords = Regex::explode(" ", query.substr(pos+1), true);
  } else if ((pos = query.find('#')) != std::string::npos) {
    // return a specific offset of the matched items
    multiplier = 1;
    index = estring(query.substr(pos+1));
    keywords = Regex::explode(" ", query.substr(0, pos), true);
  } else {
    // we only want one item - plain match
    multiplier = 1;
    index = NONE;
    keywords = Regex::explode(" ", query, true);
  }

  return keywords;
}

std::list<Item*> Container::search(const std::list<Item*>& items, const std::vector<std::string>& keywords, const int& multiplier, const int& index) {
  int counter = 0;
  std::list<Item*> matches;

  /* return everything */
  if (multiplier == ALL && index == NONE) {
    matches.insert(matches.end(), items.begin(), items.end());
  /* return all of the matched items */
  } else if (multiplier == ALL_W_KEYWORDS && index == NONE) {
    for (std::list<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(*it, keywords)) {
        matches.push_back(*it);
      }
    }
  /* return only so many of the matched items */
  } else if (multiplier > 0 && index == NONE) {
    for (std::list<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(*it, keywords)) {
        matches.push_back(*it);
        if (++counter >= multiplier) {
          break;
        }
      }
    }
  /* return a specific matched item */
  } else if (index > 0 && multiplier == 1) {
    for (std::list<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(*it, keywords) && ++counter == index) {
        matches.push_back(*it);
        break;
      }
    }
  }

  return matches;
}

std::list<Item*> Container::search(const std::map<int,Item*>& items, const std::vector<std::string>& keywords, const int& multiplier, const int& index) {
  int counter = 0;
  std::list<Item*> matches;

  /* return everything */
  if (multiplier == ALL && index == NONE) {
    for (std::map<int,Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      matches.push_back(it->second);
    }
  /* return all of the matched items */
  } else if (multiplier == ALL_W_KEYWORDS && index == NONE) {
    for (std::map<int,Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(it->second, keywords)) {
        matches.push_back(it->second);
      }
    }
  /* return only so many of the matched items */
  } else if (multiplier > 0 && index == NONE) {
    for (std::map<int,Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(it->second, keywords)) {
        matches.push_back(it->second);
        if (++counter >= multiplier) {
          break;
        }
      }
    }
  /* return a specific matched item */
  } else if (index > 0 && multiplier == 1) {
    for (std::map<int,Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
      if (commonSearch(it->second, keywords) && ++counter == index) {
        matches.push_back(it->second);
        break;
      }
    }
  }

  return matches;
}

bool Container::commonSearch(Item* item, const std::vector<std::string>& keywords) {
  for (std::vector<std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it) {
    if (!item->identifiers().matchesKeyword(*it)) {
      return false;
    }
  }
  return true;
}
