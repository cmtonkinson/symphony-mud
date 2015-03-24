
#include "enum.hpp"
#include "enum-table.hpp"

Enum::Enum(EnumInt value):
    _value(value) {
  return;
}

Enum::~Enum(void) {
  return;
}

void Enum::operator = (const Enum& ref) {
  _value = ref.number();
  _table = ref._table;
  return;
}

EnumInt Enum::number(void) const {
  return _value;
}

std::string Enum::string(void) const {
  return _table->get(_value);
}

void Enum::set(const EnumInt& value) {
  _value = value;
  return;
}

void Enum::set(const std::string& value) {
  _value = _table->get(value);
  return;
}
