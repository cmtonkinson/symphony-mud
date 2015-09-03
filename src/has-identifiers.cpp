
#include "has-identifiers.hpp"
#include "regex.hpp"

HasIdentifiers::HasIdentifiers(void) {
  return;
}

HasIdentifiers::HasIdentifiers(const HasIdentifiers& ref) {
  shortname(ref.shortname());
  longname(ref.longname());
  description(ref.description());
  setKeywords(ref.getKeywords());
  return;
}

HasIdentifiers::~HasIdentifiers(void) {
  getKeywords().clear();
  return;
}

std::string HasIdentifiers::getKeywordList(std::string glue) const {
  return Regex::implode(glue, getKeywords());
}

void HasIdentifiers::addKeyword(const std::string& keyword) {
  std::string foo = ColorString(keyword).stripColor();
  Regex::replace("[^a-z0-9]+", "", foo);
  getKeywords().insert(Regex::lower(foo));
  return;
}

void HasIdentifiers::removeKeyword(const std::string& keyword) {
  getKeywords().erase(Regex::lower(ColorString(keyword).stripColor()));
  return;
}

bool HasIdentifiers::matchesKeyword(const std::string& keyword) const {
  std::string foo = Regex::lower(ColorString(keyword).stripColor());
  for (std::set<std::string>::const_iterator it = getKeywords().begin(); it != getKeywords().end(); ++it) {
    if (Regex::strPrefix(foo, *it)) {
      return true;
    }
  }
  return false;
}

std::string HasIdentifiers::serializeKeywords(void) const {
  return Regex::implode("|", getKeywords());
}

void HasIdentifiers::unserializeKeywords(const std::string& serialization) {
  std::vector<std::string> keywords = Regex::explode("|", serialization);
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    addKeyword(*it);
  }
  return;
}

std::string HasIdentifiers::longestKeyword(void) const {
  std::string longest = "";
  int longest_length  = 0;
  int length          = 0;
  for (std::set<std::string>::iterator iter = _keywords.begin(); iter != _keywords.end(); ++iter) {
    length = (*iter).size();
    if (length > longest_length) {
      longest_length = length;
      longest = *iter;
    }
  }
  return longest;
}
