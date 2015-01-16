
#include <limits.h>
#include <math.h>
#include "datum.h"
//////////////////////////////////////////////////////////////////////////  MYSQL EXCEPTION  //////////////////////////////////////////////////////////////////////////
DatumException::DatumException(void) {
  return;
}

DatumException::DatumException(const std::string& file, const unsigned long& line, const std::string& extra) {
  char* foo = new char[extra.size()+100];
  sprintf(foo, "Datum Exception %s line %lu: %s\n", file.c_str(), line, extra.c_str());
  setMessage(foo);
  return;
}

DatumException::DatumException(const DatumException& ref) {
  setMessage(ref.getMessage());
  return;
}

DatumException::~DatumException(void) {
  return;
}

//////////////////////////////////////////////////////////////////////////  DATUM CLASS  //////////////////////////////////////////////////////////////////////////
Datum::Datum(void) {
  data(NULL);
  type(DATUM_TYPE_NULL);
  return;
}

Datum::Datum(const Datum& ref) {
  data(NULL);
  copy(ref);
  return;
}

Datum::Datum(const int& value) {
  data(NULL);
  set((long)value);
  return;
}

Datum::Datum(const long& value) {
  data(NULL);
  set(value);
  return;
}

Datum::Datum(const size_t& value) {
  data(NULL);
  set((long)value);
  return;
}

Datum::Datum(const float& value) {
  data(NULL);
  set((double)value);
  return;
}

Datum::Datum(const double& value) {
  data(NULL);
  set(value);
  return;
}

Datum::Datum(const char& value) {
  char foo[2];
  foo[0] = value;
  foo[1] = 0;
  data(NULL);
  set(foo);
  return;
}

Datum::Datum(const char* value) {
  data(NULL);
  set(value);
  return;
}

Datum::Datum(const std::string& value) {
  data(NULL);
  set(value.c_str());
  return;
}

Datum::~Datum(void) {
  clear();
  return;
}

void Datum::copy(const Datum& ref) {
  DList* l = NULL;
  DList* rl = NULL;
  switch (ref.type()) {
    case DATUM_TYPE_NULL:
      clear();
      break;
    case DATUM_TYPE_LONG:
      realloc(sizeof(long));
      memcpy(data(), ref.data(), sizeof(long));
      type(DATUM_TYPE_LONG);
      break;
    case DATUM_TYPE_DOUBLE:
      realloc(sizeof(double));
      memcpy(data(), ref.data(), sizeof(double));
      type(DATUM_TYPE_DOUBLE);
      break;
    case DATUM_TYPE_STRING:
      realloc(0);
      data(strdup((const char*)ref.data()));
      type(DATUM_TYPE_STRING);
      break;
    case DATUM_TYPE_LIST:
      realloc(sizeof(DList*));
      l = new DList();
      rl = ((DList*)ref.data());
      for (DList::const_iterator it = rl->begin(); it != rl->end(); ++it) {
        l->push_back(*it);
      }
      data(l);
      type(DATUM_TYPE_LIST);
      break;
  }
  return;
}

void Datum::realloc(const size_t& size) {
  // list preprocessing
  if (type() == DATUM_TYPE_LIST) {
    ((DList*)data())->clear();
    delete (DList*)data();
    data(NULL);
  }
  // string preprocessing
  if (type() == DATUM_TYPE_STRING) {
    free(data());
    data(NULL);
  }
  // get on with it, already...
  data(::realloc(data(), size));
  return;
}

DList::iterator Datum::get_pos(const unsigned int& index) {
  DList* l = NULL;
  DList::iterator it;
  unsigned long u = 0;

  // Get ourselves set up...
  make_list();
  l = (DList*)data();

  // Check for an out-of-bounds...
  if (index >= l->size()) {
    throw DatumException(__FILE__, __LINE__, "list index out of bounds");
  }

  if (index <= l->size()/2) {
    // Count from the beginning...
    it = l->begin();
    u = 0;
    while (u < index) {
      ++it;
      ++u;
    }
  } else {
    // Count from the end...
    it = l->end();
    u = l->size();
    while (u > index) {
      --it;
      --u;
    }
  }

  return it;
}

void Datum::set(const long& value) {
  size_t size = sizeof(value);
  realloc(size);
  memcpy(data(), &value, size);
  type(DATUM_TYPE_LONG);
  return;
}

void Datum::set(const double& value) {
  size_t size = sizeof(value);
  realloc(size);
  memcpy(data(), &value, size);
  type(DATUM_TYPE_DOUBLE);
  return;
}

void Datum::set(const char& value) {
  char foo[2];
  foo[0] = value;
  foo[1] = 0;
  realloc(2);
  strcpy((char*)data(), foo);
  type(DATUM_TYPE_STRING);
  return;
}

void Datum::set(const char* value) {
  size_t size = strlen(value)+1;
  realloc(size);
  strcpy((char*)data(), value);
  type(DATUM_TYPE_STRING);
  return;
}

bool Datum::operator == (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return false;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return true;
    case DATUM_TYPE_LONG:
      return *(long*)data() == *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() == *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) == 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us == *them;
    default:
      return false;
  }
}

bool Datum::operator != (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return true;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return false;
    case DATUM_TYPE_LONG:
      return *(long*)data() != *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() != *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) != 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us != *them;
    default:
      return false;
  }
}

bool Datum::operator < (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return false;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return false;
    case DATUM_TYPE_LONG:
      return *(long*)data() < *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() < *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) < 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us < *them;
    default:
      return false;
  }
}

bool Datum::operator <= (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return false;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return true;
    case DATUM_TYPE_LONG:
      return *(long*)data() <= *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() <= *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) <= 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us <= *them;
    default:
      return false;
  }
}

bool Datum::operator > (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return false;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return false;
    case DATUM_TYPE_LONG:
      return *(long*)data() > *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() > *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) > 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us > *them;
    default:
      return false;
  }
}

bool Datum::operator >= (const Datum& ref) const {
  DList* us = NULL;
  DList* them = NULL;
  if (type() != ref.type()) {
    return false;
  }
  switch (type()) {
    case DATUM_TYPE_NULL:
      return true;
    case DATUM_TYPE_LONG:
      return *(long*)data() >= *(long*)ref.data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data() >= *(double*)ref.data();
    case DATUM_TYPE_STRING:
      return strcmp((const char*)data(), (const char*)ref.data()) >= 0;
    case DATUM_TYPE_LIST:
      us = (DList*)data();
      them = (DList*)ref.data();
      return *us >= *them;
    default:
      return false;
  }
}

Datum& Datum::operator = (const Datum& ref) {
  copy(ref);
  return *this;
}

Datum& Datum::operator = (const int& value) {
  set((long)value);
  return *this;
}

Datum& Datum::operator = (const long& value) {
  set(value);
  return *this;
}

Datum& Datum::operator = (const size_t& value) {
  set((long)value);
  return *this;
}

Datum& Datum::operator = (const float& value) {
  set((double)value);
  return *this;
}

Datum& Datum::operator = (const double& value) {
  set(value);
  return *this;
}

Datum& Datum::operator = (const char& value) {
  set(value);
  return *this;
}

Datum& Datum::operator = (const char* value) {
  set(value);
  return *this;
}

Datum& Datum::operator = (const std::string& value) {
  set(value.c_str());
  return *this;
}

Datum::operator long(void) const {
  long l = 0;
  switch (type()) {
    case DATUM_TYPE_LONG:
      return *(long*)data();
    case DATUM_TYPE_DOUBLE:
      return floor(*(double*)data());
    case DATUM_TYPE_STRING:
      sscanf((char*)data(), "%ld", &l);
      return l;
    case DATUM_TYPE_LIST:
      return (long)size();
    default:
      return 0;
  }
}

Datum::operator double(void) const {
  double d = 0.0;
  switch (type()) {
    case DATUM_TYPE_LONG:
      return *(double*)data();
    case DATUM_TYPE_DOUBLE:
      return *(double*)data();
    case DATUM_TYPE_STRING:
      sscanf((char*)data(), "%lf", &d);
      return d;
    case DATUM_TYPE_LIST:
      return (double)size();
    default:
      return 0;
  }
}

Datum::operator const char*(void) const {
  char* buffer = NULL;
  switch (type()) {
    case DATUM_TYPE_LONG:
      buffer = new char[32];
      sprintf(buffer, "%ld", *(long*)data());
      break;
    case DATUM_TYPE_DOUBLE:
      buffer = new char[32];
      sprintf(buffer, "%f", *(double*)data());
      break;
    case DATUM_TYPE_STRING:
      buffer = strdup((char*)data());
      break;
    case DATUM_TYPE_LIST:
      return toString().c_str();
    default:
      buffer = new char;
      *buffer = 0;
      break;
  }
  return buffer;
}

Datum::operator short(void) const {
  long l = this->operator long();
  if (l > SHRT_MAX) {
    return SHRT_MAX;
  }
  return l < SHRT_MIN ? SHRT_MIN : l;
}

Datum::operator int(void) const {
  long l = this->operator long();
  if (l > INT_MAX) {
    return INT_MAX;
  }
  return l < INT_MIN ? INT_MIN : l;
}

Datum::operator unsigned short(void) const {
  long l = this->operator long();
  if (l > USHRT_MAX) {
    return USHRT_MAX;
  }
  return l < 0 ? 0 : l;
}

Datum::operator unsigned int(void) const {
  long l = this->operator long();
  if (l > UINT_MAX) {
    return UINT_MAX;
  }
  return l < 0 ? 0 : l;
}

Datum::operator unsigned long(void) const {
  long l = this->operator long();
  return l < 0 ? 0 : l;
}

void Datum::make_list(void) {
  DList* l = NULL;

  // If it's already an list, we've got no work to do...
  if (type() == DATUM_TYPE_LIST) {
    return;
  }

  l = new DList();
  if (type() != DATUM_TYPE_NULL) {
    l->push_back(*this);
  }
  realloc(sizeof(DList*));
  data(l);
  type(DATUM_TYPE_LIST);

  return;
}

Datum& Datum::append(const Datum& ref) {
  make_list();
  ((DList*)data())->push_back(ref);
  return *this;
}

Datum& Datum::append_list(Datum& ref) {
  DList* l1 = NULL;
  DList* l2 = NULL;
  make_list();
  ref.make_list();
  l1 = (DList*)data();
  l2 = (DList*)ref.data();
  l1->insert(l1->end(), l2->begin(), l2->end());
  return *this;
}

Datum& Datum::at(const unsigned long& index) {
  return *get_pos(index);
}

Datum& Datum::insert(const Datum& ref, const unsigned long& index) {
  DList::iterator pos = get_pos(index);
  ((DList*)data())->insert(pos, ref);
  return *this;
}

Datum& Datum::remove(const unsigned long& index) {
  ((DList*)data())->erase(get_pos(index));
  return *this;
}

Datum& Datum::first(void) {
  make_list();
  return ((DList*)data())->front();
}

Datum& Datum::last(void) {
  make_list();
  return ((DList*)data())->back();
}

Datum& Datum::reverse(void) {
  make_list();
  ((DList*)data())->reverse();
  return *this;
}

unsigned Datum::size(void) {
  make_list();
  return ((DList*)data())->size();
}

unsigned Datum::size(void) const {
  // Only use this method when you know the Datum is a list
  // because it does no typechecking.
  return ((DList*)data())->size();
}

long Datum::longVal(void) const {
  if (type() == DATUM_TYPE_LONG) {
    return *(long*)data();
  } else {
    Datum foo = *this;
    return (long)foo;
  }
}

double Datum::doubleVal(void) const {
  if (type() == DATUM_TYPE_DOUBLE) {
    return *(double*)data();
  } else {
    Datum foo = *this;
    return (double)foo;
  }
}

const char* Datum::stringVal(void) const {
  if (type() == DATUM_TYPE_STRING) {
    return (const char*)data();
  } else {
    Datum foo = *this;
    return (const char*)foo;
  }
}

bool Datum::truth(void) const {
  // "cast" the Datum to a bool
  // for numeric types, zero is false, non-zero true
  // for string and list, empty is false, non-empty is true
  switch (type()) {
    case DATUM_TYPE_LONG:
      return *(long*)data() != 0;
    case DATUM_TYPE_DOUBLE:
      return *(long*)data() != 0.0;
    case DATUM_TYPE_STRING:
      return strlen((const char*)data()) != 0;
    case DATUM_TYPE_LIST:
      return !((DList*)data())->empty();
    default:
      return false;
  }
}

void Datum::clear(void) {
  type(DATUM_TYPE_NULL);
  realloc(0);
  return;
}

void Datum::upcast(Datum& left, Datum& right) {
  Datum* a = NULL;
  Datum* b = NULL;
  if (left.type() != right.type()) {
    // first, get the more basic Datum into 'a' and the more complex into 'b'
    a = left.type() < right.type() ? &left : &right;
    b = left.type() < right.type() ? &right : &left;
    // now, we just need to cast a to the type of b
    switch (b->type()) {
      case DATUM_TYPE_LONG:
        *a = (long)*a;
        break;
      case DATUM_TYPE_DOUBLE:
        *a = (double)*a;
        break;
      case DATUM_TYPE_STRING:
        *a = (const char*)*a;
        break;
      case DATUM_TYPE_LIST:
        a->make_list();
        break;
      default:
        break;
    }
  }
  return;
}

const char* Datum::stringType(void) const {
  switch (type()) {
    case DATUM_TYPE_LONG:     return "long";
    case DATUM_TYPE_DOUBLE:   return "double";
    case DATUM_TYPE_STRING:   return "string";
    case DATUM_TYPE_LIST:     return "list";
    default:                  return "undefined";
  }
}

void Datum::print(void) const {
  printf("%s", toString().c_str());
  return;
}

std::string Datum::toString(void) const {
  std::string dest;
  char buf[64];
  DList* l = NULL;
  DList::const_iterator it;
  DList::const_iterator end;
  switch (type()) {
    case DATUM_TYPE_LONG:
      sprintf(buf, "%ld", *((long*)data()));
      dest = buf;
      break;
    case DATUM_TYPE_DOUBLE:
      sprintf(buf, "%f", *((double*)data()));
      dest = buf;
      break;
    case DATUM_TYPE_STRING:
      dest.assign((char*)data());
      break;
    case DATUM_TYPE_LIST:
      dest.assign("{ ");
      l = (DList*)data();
      end = l->end();
      for (it = l->begin(); it != end; ++it) {
        dest.append(it->toString());
        dest.append(", ");
      }
      dest.resize(dest.size()-2);
      dest.append(" }");
      break;
    default:
      break;
  }
  return dest;
}

void Datum::prettyPrint(unsigned depth) const {
  DList* l = NULL;
  DList::const_iterator it;
  DList::const_iterator end;
  for (unsigned u = 0; u < depth; ++u) printf(" ");
  switch (type()) {
    case DATUM_TYPE_LONG:
      printf("[long] = %ld", *((signed long*)data()));
      break;
    case DATUM_TYPE_DOUBLE:
      printf("[double] = %f", *((double*)data()));
      break;
    case DATUM_TYPE_STRING:
      printf("[string] = \"%s\"", (char*)data());
      break;
    case DATUM_TYPE_LIST:
      printf("[list] = {\n");
      l = (DList*)data();
      end = l->end();
      for (it = l->begin(); it != end; ++it) it->prettyPrint(depth + 10);
      for (unsigned u = 0; u < depth + 10; ++u) printf(" ");
      printf("}");
      break;
    default:
      break;
  }
  printf(" <%p>\n", data());
  return;
}

std::string Datum::serialize(void) const {
  std::string out;
  char buf[64];
  DList* l = NULL;
  DList::const_iterator it;
  DList::const_iterator end;
  switch (type()) {
    case DATUM_TYPE_LONG:
      sprintf(buf, "%c%ld", DATUM_SERIAL_LONG, *((long*)data()));
      out = buf;
      break;
    case DATUM_TYPE_DOUBLE:
      sprintf(buf, "%c%f", DATUM_SERIAL_DOUBLE, *((double*)data()));
      out = buf;
      break;
    case DATUM_TYPE_STRING:
      sprintf(buf, "%c%d:", DATUM_SERIAL_STRING, (int)strlen((char*)data()));
      out.assign(buf).append((char*)data());
      break;
    case DATUM_TYPE_LIST:
      l = (DList*)data();
      end = l->end();
      sprintf(buf, "%c%d", DATUM_SERIAL_LIST, (int)l->size());
      out.assign(buf);
      for (it = l->begin(); it != end; ++it) out.append(it->serialize()).append(",");
      out.resize(out.size()-1);
      break;
    default:
      break;
  }
  return out;
}

Datum Datum::unserialize(const char* serialized) {
  int start = 0;
  return unserialize(serialized, start);
}

Datum Datum::unserialize(const char* serialized, int& position) {
  Datum d;
  char type = serialized[position++]; // get the type tag
  long l = 0;
  double f = 0;
  int len = 0;
  int captured = 0;
  char* string = NULL;
  switch (type) {
    case DATUM_SERIAL_LONG:
      sscanf(serialized + position, "%ld%n", &l, &captured);
      position += captured;
      d = l;
      break;
    case DATUM_SERIAL_DOUBLE:
      sscanf(serialized + position, "%lf%n", &f, &captured);
      position += captured;
      d = f;
      break;
    case DATUM_SERIAL_STRING:
      sscanf(serialized + position, "%d%n", &len, &captured);
      position += captured + 1; // don't forget the colon
      string = strndup(serialized + position, len);
      position += len;
      d = string;
      free(string);
      break;
    case DATUM_SERIAL_LIST:
      d.make_list();
      sscanf(serialized + position, "%d%n", &len, &captured);
      position += captured;
      for (int i = 0; i < len; ++i) d.append(unserialize(serialized, position));
      break;
    default:
      break;
  }
  return d;
}
