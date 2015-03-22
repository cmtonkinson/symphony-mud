
#include "currency.h"
#include "estring.h"

const unsigned Currency::SILVER_PER_GOLD;

Currency::Currency(void): _value(0) {
  return;
}

Currency::Currency(unsigned value): _value(value) {
  return;
}

Currency::Currency(const Currency& other): _value(other.value()) {
  return;
}

Currency::~Currency(void) {
  return;
}

unsigned Currency::silver(void) const {
  return value() % SILVER_PER_GOLD;
}

unsigned Currency::gold(void) const {
  return value() / SILVER_PER_GOLD;
}

std::string Currency::shortString(void) const {
  std::string dest;

  // Nothing
  if (value() == 0) {
    return "0";
  // Just silver
  } else if (value() < SILVER_PER_GOLD) {
    dest << "{W";
    dest << estring(silver());
    dest << "{x";
  // Just gold
  } else if (silver() == 0) {
    dest << "{y";
    dest << estring(gold());
    dest << "{x";
  // Silver and gold
  } else {
    dest << "{W";
    dest << estring(silver());
    dest << "{x, {y";
    dest << estring(gold());
    dest << "{x";
  }

  return dest;
}

std::string Currency::longString(void) const {
  std::string dest;

  // Nothing
  if (value() == 0) {
    return "0";
  // Just silver
  } else if (value() < SILVER_PER_GOLD) {
    dest << "{W";
    dest << estring(silver());
    dest << "{x silver";
  // Just gold
  } else if (silver() == 0) {
    dest << "{y";
    dest << estring(gold());
    dest << "{x gold";
  // Silver and gold
  } else {
    dest << "{W";
    dest << estring(silver());
    dest << "{x silver and {y";
    dest << estring(gold());
    dest << "{x gold";
  }

  return dest;
}
