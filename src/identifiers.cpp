
#include "identifiers.h"
#include "regex.h"

Identifiers::Identifiers(void) {
  return;
}

Identifiers::Identifiers(const Identifiers& ref) {
  shortname(ref.shortname());
  longname(ref.longname());
  description(ref.description());
  setKeywords(ref.getKeywords());
  return;
}

Identifiers::~Identifiers(void) {
  getKeywords().clear();
  return;
}

std::string Identifiers::getKeywordList(std::string glue) const {
  return Regex::implode(glue, getKeywords());
}

void Identifiers::addKeyword(const std::string& keyword) {
  std::string foo = ColorString(keyword).stripColor();
  Regex::replace("[^a-z0-9]+", "", foo);
  getKeywords().insert(Regex::lower(foo));
  return;
}

void Identifiers::removeKeyword(const std::string& keyword) {
  getKeywords().erase(Regex::lower(ColorString(keyword).stripColor()));
  return;
}

bool Identifiers::matchesKeyword(const std::string& keyword) const {
  std::string foo = Regex::lower(ColorString(keyword).stripColor());
  for (std::set<std::string>::const_iterator it = getKeywords().begin(); it != getKeywords().end(); ++it) {
    if (Regex::strPrefix(foo, *it)) {
      return true;
    }
  }
  return false;
}

std::string Identifiers::serializeKeywords(void) const {
  return Regex::implode("|", getKeywords());
}

void Identifiers::unserializeKeywords(const std::string& serialization) {
  std::vector<std::string> keywords = Regex::explode("|", serialization);
  for (std::vector<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
    addKeyword(*it);
  }
  return;
}

std::string Identifiers::longestKeyword(void) const {
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
