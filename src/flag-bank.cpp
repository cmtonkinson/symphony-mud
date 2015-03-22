
#include "flag-bank.h"
#include "flag-table.h"

FlagBank::FlagBank(FlagInt bank):
    _bank(bank) {
  return;
}

FlagBank::FlagBank(const FlagBank& ref):
    _bank(ref.bank()) {
  return;
}

FlagBank::~FlagBank(void) {
  return;
}

std::string FlagBank::list(const FlagTable& table, bool decorative) const {
  std::string buf;
  const ValToName* map = decorative ? &table.decorativeName() : &table.valueToName();
  for (ValToName::const_iterator it = map->begin(); it != map->end(); ++it) {
    if (test(it->first)) {
      buf.append(it->second).append(1, ' ');
    }
  }
  if (!buf.empty()) {
    buf.resize(buf.size()-1);
  }
  return buf;
}
