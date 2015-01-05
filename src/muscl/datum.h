/*
 * $Id: datum.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_MUSCL_DATUM_
#define H_MUSCL_DATUM_

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#define DATUM_TYPE_NULL     1
#define DATUM_TYPE_LONG     2
#define DATUM_TYPE_DOUBLE   4
#define DATUM_TYPE_STRING   8
#define DATUM_TYPE_LIST     16

#define DATUM_SERIAL_EMPTY    'n'
#define DATUM_SERIAL_LONG     'i'
#define DATUM_SERIAL_DOUBLE   'r'
#define DATUM_SERIAL_STRING   's'
#define DATUM_SERIAL_LIST     'l'

class Datum;
typedef std::list<Datum> DList;
typedef std::list<Datum>::const_iterator DListIter;

//////////////////////////////////////////////////////////////////////////  DATUM EXCEPTION  //////////////////////////////////////////////////////////////////////////
class DatumException {
  private:
    std::string     _message;

    void  setMessage(const std::string& message) { _message = message; return; }

  public:
    DatumException(void);
    DatumException(const std::string& file, const unsigned long& line, const std::string& extra = "");
    DatumException(const DatumException& ref);
    ~DatumException(void);

    std::string    getMessage(void) const { return _message; }
};

//////////////////////////////////////////////////////////////////////////  DATUM CLASS  //////////////////////////////////////////////////////////////////////////
class Datum {
  public:
    // Constructors...
    Datum(void);
    Datum(const Datum& ref);
    Datum(const int& value);
    Datum(const long& value);
    Datum(const size_t& value);
    Datum(const float& value);
    Datum(const double& value);
    Datum(const char& value);
    Datum(const char* value);
    Datum(const std::string& value);
    ~Datum(void);

    // operators...
    bool      operator == (const Datum& ref) const;
    bool      operator != (const Datum& ref) const;
    bool      operator <  (const Datum& ref) const;
    bool      operator <= (const Datum& ref) const;
    bool      operator >  (const Datum& ref) const;
    bool      operator >= (const Datum& ref) const;
    Datum&    operator =  (const Datum& ref);
    Datum&    operator =  (const int& value);
    Datum&    operator =  (const long& value);
    Datum&    operator =  (const size_t& value);
    Datum&    operator =  (const float& value);
    Datum&    operator =  (const double& value);
    Datum&    operator =  (const char& value);
    Datum&    operator =  (const char* value);
    Datum&    operator =  (const std::string& value);
    operator  long        (void) const;
    operator  double      (void) const;
    operator  const char* (void) const;

    // convenience operators
    // Note: Since the internal storage mechanism for a Datum "integer" type
    //       is a signed long int, these conversion operators will perform
    //       simple bounds checking against the MIN and MAX of the type to which
    //       the value is being converted.  Values outside of the valid range
    //       for the target type will be converted to either MIN or MAX,
    //       depending upon whether the value is respectively below or above the
    //       range of the target type.
    operator  short           (void) const;
    operator  int             (void) const;
    operator  unsigned short  (void) const;
    operator  unsigned int    (void) const;
    operator  unsigned long   (void) const;

    // Public accessor methods...
    void  type(const short& type)       { _type = type; }
    short type(void) const              { return _type; }
    void  data(void* data)              { _data = data; }
    void* data(void) const              { return _data; }

    // List methods...
    void      make_list(void);
    Datum&    append(const Datum& ref);
    Datum&    append_list(Datum& ref);
    Datum&    at(const unsigned long& index);
    Datum&    insert(const Datum& ref, const unsigned long& index);
    Datum&    remove(const unsigned long& index);
    Datum&    first(void);
    Datum&    last(void);
    Datum&    reverse(void);
    unsigned  size(void);
    unsigned  size(void) const;

    // these methods provide "type coversion" for CONST objects
    // note: if the object being "converted" is known to be the type desired, these
    //       methods are pretty fast.  If the object is a different type though,
    //       these methods make a value copy and non-const-ly convert THAT object
    //       which takes some overhead (the most costly being creating said value copy)
    long        longVal(void) const;
    double      doubleVal(void) const;
    const char* stringVal(void) const;

    // General methods...
    bool          truth(void) const;
    void          clear(void);
    const char*   stringType(void) const;
    void          print(void) const;
    std::string   toString(void) const;
    void          prettyPrint(unsigned depth = 0) const;
    std::string   serialize(void) const;

    // Statics...
    static void   mask(const Datum& d, unsigned& u);
    static void   upcast(Datum& left, Datum& right);
    static Datum  unserialize(const char* serialized);
    static Datum  unserialize(const char* serialized, int& position);

  private:
    // data
    int             _type;
    void*           _data;

    // methods
    void            set(const long& value);
    void            set(const double& value);
    void            set(const char& value);
    void            set(const char* value);
    void            copy(const Datum& ref);
    void            realloc(const size_t& size);
    DList::iterator get_pos(const unsigned int& index);
};

#endif // End of inclusion guard
